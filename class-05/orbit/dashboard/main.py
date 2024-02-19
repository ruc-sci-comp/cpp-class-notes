"""Orbit!
"""
import copy
import io
import pathlib
import subprocess

import holoviews as hv
import hvplot.pandas
import numpy as np
import pandas as pd
import panel as pn

hv.extension('bokeh')
pn.extension()


main = str(pathlib.Path(__file__).parents[1] / 'bin' / 'main.exe')

def load_data():
    proc = subprocess.run([main], capture_output=True)
    stdout = proc.stdout.decode()
    return pd.read_csv(io.StringIO(stdout), names=['t', 'x', 'y', 'vx', 'vy'], delimiter=' ')


state = load_data()
state['magnitudes'] = np.abs(np.hypot(state.vx, state.vy)) * 0.1
state['angles'] = np.arctan2(state.vy, state.vx)
time_steps = state.t.unique()


player = pn.widgets.Player(
    name='Player',
    start=0,
    end=len(time_steps),
    value=0,
    interval=33,
    width_policy='max'
)
time_box = pn.widgets.TextInput(name='time (s)', disabled=True)
fps_input = pn.widgets.IntSlider(name='FPS', start=1, end=30, step=1, value=30)


@pn.depends(fps_input.param.value_throttled, watch=True)
def update_fps(value):
    player.interval = 1000//value
    player.pause()
    player.play()


sun = hv.Scatter([[0.0, 0.0]]).opts(size=50)


@pn.depends(player)
def plot(value):
    time_box.value = str(value)

    scatter = hv.Scatter([state.iloc[value]], kdims=['x'], vdims=['y']).opts(
        size=10
    )

    trace = hv.Curve(state.iloc[:value], kdims=['x'], vdims=['y'])

    vector = hv.VectorField([state.iloc[value]], kdims=['x', 'y'], vdims=['angles', 'magnitudes']).opts(
        rescale_lengths=False,
        arrow_heads=False,
        pivot='tail',
        magnitude=hv.dim('magnitudes')
    )
    return sun * scatter * trace * vector

point_dmap = hv.DynamicMap(plot).opts(
    xlim=(-200e9, 200e9),
    ylim=(-200e9, 200e9),
    show_grid=True,
    frame_height=600,
    frame_width=600
)

sim_data_card = pn.Card(
    time_box,
    collapsible=False,
    title='Simulation Data'
)

sim_config_card = pn.Card(
    fps_input,
    collapsible=False,
    title='Simulation Controls'
)

description_pane = pn.pane.Markdown('''
### Inputs & Controls

* FPS - frames per second, or how quickly we playback the simulation

### Usage

* Use the player controls to play/pause the simulation at different speeds. 
* Optionally, set the FPS to change the speed of the simulation.
''')


pn.template.FastListTemplate(
    theme=pn.template.DarkTheme,
    header_background='#DC143C',
    accent_base_color='#708090',
    site='Orbit',
    title='One entity orbiting another',
    main=[
        'Simulating different integration techniques and their error.',
        pn.Row(pn.Column(point_dmap, player), description_pane),
    ],
    sidebar=[
        sim_data_card,
        sim_config_card
    ]
).servable()
