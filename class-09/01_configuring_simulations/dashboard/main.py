"""Not-So-Live Simulation
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
state = None
time_steps = None


reset_button = pn.widgets.Button(name='Reset')
time_box = pn.widgets.TextInput(name='time (s)', disabled=True)
fps_input = pn.widgets.IntSlider(name='FPS', start=1, end=30, step=1, value=30)
player = pn.widgets.DiscretePlayer(
    name='Player',
    value=0,
    interval=33,
    width_policy='max'
)


def load_data():
    proc = subprocess.run([main], capture_output=True)
    stdout = proc.stdout.decode()
    return pd.read_csv(io.StringIO(stdout), names=['t', 'x', 'y'], delimiter=',')


@pn.depends(reset_button, watch=True)
def reset(event):
    global state, time_steps
    state = load_data()
    time_steps = state.t.unique()
    player.options=time_steps.tolist()
    player.value=time_steps.tolist()[0]
    time_box.value = '0.00'


@pn.depends(fps_input.param.value_throttled, watch=True)
def update_fps(value):
    player.interval = 1000//value
    player.pause()
    player.play()


@pn.depends(player)
def plot(value):
    time_box.value = f'{value:.2f}'
    return hv.Scatter(state.loc[state.t == value], 'x', 'y')


point_dmap = hv.DynamicMap(plot).opts(
    xlim=(-25, 25),
    ylim=(-25, 25),
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
    reset_button,
    fps_input,
    collapsible=False,
    title='Simulation Controls'
)

reset(None)

pn.template.FastListTemplate(
    theme=pn.template.DarkTheme,
    header_background='#DC143C',
    accent_base_color='#708090',
    site="ruc-sci-comp-sim",
    title="simple-sim v0.1",
    main=[
        'Simulating how a worker might mine a mineral.',
        pn.Column(point_dmap, player),
    ],
    sidebar=[
        sim_data_card,
        sim_config_card
    ]
).servable()
