"""Mine!
"""
import io
import pathlib
import subprocess

import holoviews as hv
import pandas as pd
import panel as pn

hv.extension('bokeh')
pn.extension()


main = str(pathlib.Path(__file__).parents[1] / 'bin' / 'main.exe')

def load_data():
    proc = subprocess.run([main], capture_output=True)
    stdout = proc.stdout.decode()
    return pd.read_csv(io.StringIO(stdout), names=['t', 'i', 'x', 'y', 'u'], delimiter=' ')


state = load_data()
time_steps = state.t.unique()

player = pn.widgets.DiscretePlayer(
    name='Player',
    options=time_steps.tolist(),
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


@pn.depends(player)
def plot(value):
    time_box.value = f'{value:.2f}'

    scatter = hv.Scatter(state.loc[state.t == value], kdims=['x'], vdims=['y', 'i', 'u']).opts(
        size=hv.dim('u')+1.0,
        color='i',
        line_color='white',
        cmap={
            'worker': 'pink',
            'mineral': 'lightblue',
        }
    )

    return scatter

point_dmap = hv.DynamicMap(plot).opts(
    xlim=(-5, 15),
    ylim=(-5, 15),
    show_grid=True,
    show_legend=False,
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
    site='Miners!',
    title='One entity mining another',
    main=[
        'Simulating how a worker might mine a mineral.',
        pn.Row(pn.Column(point_dmap, player), description_pane),
    ],
    sidebar=[
        sim_data_card,
        sim_config_card
    ]
).servable()
