"""Basic cannonball dashboard
"""
import json
import pathlib

import holoviews as hv
import pandas as pd
import panel as pn

from .model import Model

configuration_file = pathlib.Path(__file__).parents[1] / 'data' / 'config.json'
config = json.load(open(configuration_file))

dfstream = hv.streams.Pipe(data=pd.DataFrame({'x':[], 'y':[]}))
dmap = hv.DynamicMap(hv.Scatter, streams=[dfstream]).opts(
    height=600,
    width=600
)

model_state = Model(str(configuration_file))

global_time = 0.0
global_time_delta = 0.25
periodic_callback = None
def run_model():
    if model_state.entities:
        global global_time
        global_time += global_time_delta
        model_state.update(global_time_delta)
        entities_state = pd.DataFrame([entity.position for entity in model_state.entities], columns=['x', 'y'])
        time_box.value = f'{global_time}'
        dfstream.send(entities_state)
    else:
        periodic_callback.stop()
        play_button.name = 'Play'

def play(event):
    global periodic_callback, state
    if periodic_callback is None or not periodic_callback.running:
        play_button.name = 'Stop'
        periodic_callback = pn.state.add_periodic_callback(run_model, period=1000//int(fps_input.value))
    elif periodic_callback.running:
        play_button.name = 'Play'
        periodic_callback.stop()

def reset(event):
    global model_state
    model_state.finalize()
    model_state = Model(str(configuration_file))
    entities_state = pd.DataFrame([entity.position for entity in model_state.entities], columns=['x', 'y'])
    entities_state['c'] = 'white'
    dfstream.send(entities_state)

reset(None)

time_box = pn.widgets.TextInput(name='time (s)', disabled=True)

fps_input = pn.widgets.IntSlider(name='FPS', start=1, end=30, step=1, value=30)

play_button = pn.widgets.Button(name='Play')
play_button.on_click(play)

reset_button = pn.widgets.Button(name='Reset')
reset_button.on_click(reset)

sim_data_card = pn.Card(
    time_box,
    collapsible=False,
    title='Simulation Data'
)

sim_config_card = pn.Card(
    fps_input,
    reset_button,
    play_button,
    collapsible=False,
    title='Simulation Controls'
)

description_pane = pn.pane.Markdown('''

### Inputs & Controls

* FPS - frames per second, or how quickly we playback the simulation (use 4 FPS for realtime)

### Usage

*

### Notes

*
''')

pn.template.FastListTemplate(
    theme=pn.template.DarkTheme,
    header_background='#DC143C',
    accent_base_color='#708090',
    site="ruc-sci-comp-sim",
    title="simple-sim v1.0",
    main=[
        pn.Row(dmap, description_pane)
    ],
    sidebar=[
        sim_data_card,
        sim_config_card
    ]
).servable()