"""Live Simulation
"""
import json
import pathlib

import holoviews as hv
import pandas as pd
import panel as pn

from .model import Model

hv.extension('bokeh')
pn.extension()

configuration_file = pathlib.Path(__file__).parents[1] / 'data' / 'config.json'
configuration = json.load(open(configuration_file))
model_state = Model()

boundaries = (
    model_state.environment.x_limit[0],
    model_state.environment.y_limit[0],
    model_state.environment.x_limit[1],
    model_state.environment.y_limit[1]
)
boundaries = hv.Rectangles(boundaries).opts(fill_color=None, line_color='grey')

def plot(data):
    entities, food = data
    entity_plot = hv.Scatter(entities).opts(color='black')
    food_plot = hv.Scatter(food, kdims=['x'], vdims=['y', 'q']).opts(color='q', alpha='q', size=hv.dim('q') + 1)
    return entity_plot * food_plot

dfstream = hv.streams.Pipe(data=pd.DataFrame({'x':[], 'y':[]}))
dmap = (hv.DynamicMap(plot, streams=[dfstream]) * boundaries).opts(
    height=600,
    width=600,
    bgcolor='#5C4033'
)

periodic_callback = None

play_button = pn.widgets.Button(name='Play')
reset_button = pn.widgets.Button(name='Reset')
time_box = pn.widgets.TextInput(name='time (s)', disabled=True)
fps_input = pn.widgets.IntSlider(name='FPS', start=1, end=30, step=1, value=30)

def run_model():
    global global_time
    model_state.update(configuration['delta_time'])
    entities_state = pd.DataFrame([entity.position for entity in model_state.entities], columns=['x', 'y'])
    food_state = pd.DataFrame([[*food.position, food.quantity] for food in model_state.food], columns=['x', 'y', 'q'])
    time_box.value = f'{model_state.time:.2f}'
    dfstream.send((entities_state, food_state))

@pn.depends(play_button, watch=True)
def play(value):
    global periodic_callback
    if periodic_callback is None or not periodic_callback.running:
        play_button.name = 'Stop'
        periodic_callback = pn.state.add_periodic_callback(run_model, period=1000//int(fps_input.value))
    elif periodic_callback.running:
        play_button.name = 'Play'
        periodic_callback.stop()

@pn.depends(reset_button, watch=True)
def reset(value):
    if periodic_callback:
        play_button.name = 'Play'
        periodic_callback.stop()
    model_state.finalize()
    model_state.initialize(str(configuration_file))
    entities_state = pd.DataFrame([entity.position for entity in model_state.entities], columns=['x', 'y'])
    food_state = pd.DataFrame([[*food.position, food.quantity] for food in model_state.food], columns=['x', 'y', 'q'])
    time_box.value = f'{model_state.time:.2f}'
    dfstream.send((entities_state, food_state))

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

reset(None)

pn.template.FastListTemplate(
    theme=pn.template.DarkTheme,
    header_background='#DC143C',
    accent_base_color='#708090',
    site="ruc-sci-comp-sim",
    title="Ants",
    main=[
        dmap
    ],
    sidebar=[
        sim_data_card,
        sim_config_card
    ]
).servable()