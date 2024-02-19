"""Bounce!
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
    return pd.read_csv(io.StringIO(stdout), names=['t', 'id', 'x', 'y', 'v'], delimiter=' ')

state = load_data()
state['magnitudes'] = np.abs(state.v) * 0.1
state['angles'] = np.sign(state.v) * np.pi / 2.0
time_steps = state.t.unique()
state = state.groupby('t')


discrete_player = pn.widgets.DiscretePlayer(
    name='Discrete Player',
    options=time_steps.tolist(),
    value=0,
    interval=33,
    width_policy='max'
)
time_box = pn.widgets.TextInput(name='time (s)', disabled=True)
fps_input = pn.widgets.IntSlider(name='FPS', start=1, end=30, step=1, value=30)


@pn.depends(fps_input.param.value_throttled, watch=True)
def update_fps(value):
    discrete_player.interval = 1000//value
    discrete_player.pause()
    discrete_player.play()


@pn.depends(discrete_player)
def plot(value):
    time_box.value = str(value)

    data = state.get_group(value)

    scatter = hv.Scatter(data, kdims=['x'], vdims=['y', 'id']).opts(
        size=10,
        color='id',
        colorbar=False,
        cmap='RdYlGn',
    )

    vector = hv.VectorField(data, kdims=['x', 'y'], vdims=['angles', 'magnitudes', 'id']).opts(
        rescale_lengths=False,
        arrow_heads=False,
        pivot='tail',
        magnitude=hv.dim('magnitudes'),
        color='id',
        colorbar=False,
        cmap='RdYlGn',
    )
    return scatter * vector

point_dmap = hv.DynamicMap(plot).opts(
    show_grid=True,
    frame_height=600,
    frame_width=600,
    ylim=(-1,101)
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
    site='Bounce',
    title='Multiple Entities Bouncing!',
    main=[
        'Simulating multiple entities experiencing inelastic collisions with the ground.',
        pn.Row(pn.Column(point_dmap, discrete_player), description_pane),
    ],
    sidebar=[
        sim_data_card,
        sim_config_card
    ]
).servable()
