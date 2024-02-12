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
periodic_callback = None


def load_data():
    proc = subprocess.run([main], capture_output=True)
    stdout = proc.stdout.decode()
    return pd.read_csv(io.StringIO(stdout), names=['t', 'id', 'x', 'y'], delimiter=' ')


def plot(data):
    return hv.Scatter(data, kdims=['x'], vdims=['y', 'id']).opts(
        size=10,
        color='id',
        colorbar=False,
        cmap='RdYlGn',
    )


state = load_data()
time_steps = (t for t in state.t.unique())

dfstream = hv.streams.Pipe(data=state[state.t == next(time_steps)])
point_dmap = hv.DynamicMap(plot, streams=[dfstream]).opts(
    frame_height=600,
    frame_width=600,
    ylim=(-1,101)
)


def increment_system():
    t = next(time_steps)
    dfstream.send(state[state.t == t])
    time_box.value = str(t)
    if not state:
        play_button.name = 'Play'
        periodic_callback.stop()


def play(event):
    global periodic_callback, state, time_steps
    if periodic_callback is None or not periodic_callback.running:
        state = load_data()
        time_steps = (t for t in state.t.unique())
        play_button.name = 'Stop'
        periodic_callback = pn.state.add_periodic_callback(increment_system, period=1000//int(fps_input.value))
    elif periodic_callback.running:
        play_button.name = 'Play'
        periodic_callback.stop()

time_box = pn.widgets.TextInput(name='time (s)', disabled=True)

fps_input = pn.widgets.IntSlider(name='FPS', start=1, end=30, step=1, value=30)

play_button = pn.widgets.Button(name='Play')
play_button.on_click(play)


sim_data_card = pn.Card(
    time_box,
    collapsible=False,
    title='Simulation Data'
)

sim_config_card = pn.Card(
    fps_input,
    play_button,
    collapsible=False,
    title='Simulation Controls'
)

description_pane = pn.pane.Markdown('''

### Inputs & Controls

* Enable Trace - turns on a simple movement trace
* FPS - frames per second, or how quickly we playback the simulation

### Usage

* Click *Play* to start the simulation; then *Stop* to end the simulation. 
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
        pn.Row(point_dmap, description_pane),
    ],
    sidebar=[
        sim_data_card,
        sim_config_card
    ]
).servable()
