"""Live Simulation
"""
import json
import pathlib

import holoviews as hv
import numpy as np
import pandas as pd
import panel as pn

from .model import Model
from .components.live_player import LivePlayer

hv.extension('bokeh')
pn.extension()

entity_columns = ['x', 'y', 'm']
configuration_file = pathlib.Path(__file__).parents[1] / 'data' / 'config.json'
configuration = json.load(open(configuration_file))
model_state = Model()


def plot(data):
    return hv.Points(data, vdims=['m']).opts(color='m', cmap='plasma', size=10)


def run_model():
    model_state.update(configuration['delta_time'])
    entities = pd.concat([
        pd.DataFrame([[*e.position, e.mass] for e in model_state.entities], columns=entity_columns),
    ])
    time_box.value = f'{model_state.time:.2f}'
    dfstream.send(entities)


def reset_model():
    model_state.finalize()
    global configuration 
    configuration = json.load(open(configuration_file))
    try:
        model_state.initialize(str(configuration_file), configuration['seed'])
    except TypeError:
        model_state.initialize(str(configuration_file))
    entities = pd.concat([
        pd.DataFrame([[*e.position, e.mass] for e in model_state.entities], columns=entity_columns),
    ])
    time_box.value = f'{model_state.time:.2f}'
    dfstream.send(entities)


dfstream = hv.streams.Pipe(data=pd.DataFrame({'x':[], 'y':[]}))
dmap = hv.DynamicMap(plot, streams=[dfstream])


live_player = LivePlayer(run_model, reset_model)
time_box = pn.widgets.TextInput(name='time (s)', disabled=True)

sim_data_card = pn.Card(
    time_box,
    collapsible=False,
    title='Simulation Data'
)

sim_config_card = pn.Card(
    live_player,
    collapsible=False,
    title='Simulation Controls'
)

live_player.reset()


app = pn.template.FastListTemplate(
    theme=pn.template.DarkTheme,
    header_background='#DC143C',
    accent_base_color='#708090',
    site="ruc-sci-comp-sim",
    title="N-Body Problem",
    main=[
        dmap.opts(
            xlim=(-200, 200),
            ylim=(-200, 200),
            height=600,
            width=600,
            show_grid=True
        )
    ],
    sidebar=[
        sim_config_card,
        sim_data_card
    ]
).servable()