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

projectile_columns = ['x', 'y', 'vx', 'vy', 'type']
configuration_file = pathlib.Path(__file__).parents[1] / 'data' / 'config.json'
configuration = json.load(open(configuration_file))
model_state = Model()


def plot(data):
    return hv.Points(data, vdims=['vx', 'vy', 'type']).opts(color='type', size=10, cmap={
        'threat': 'red',
        'track': 'yellow',
        'interceptor': 'lightblue',
    })


def plot_scenario(data):
    radar, weapons = data
    radar_plot = hv.Ellipse(*radar.position, radar.detection_range * 2.0).opts(color='lightgreen') * hv.Points([radar.position]).opts(color='lightgreen', size=15)
    weapons_plot = hv.Ellipse(*weapons.position, weapons.engage_range * 2.0).opts(color='red') * hv.Points([weapons.position]).opts(color='red', size=5)
    return radar_plot * weapons_plot

def run_model():
    global global_time
    model_state.update(configuration['delta_time'])
    all_projectiles = pd.concat([
        pd.DataFrame([[*p.position, *p.velocity, 'threat'] for p in model_state.projectiles], columns=projectile_columns),
        pd.DataFrame([[*p.position, *p.velocity, 'interceptor'] for p in model_state.weapons_system.interceptors], columns=projectile_columns),
        pd.DataFrame([[*p.position, *p.velocity, 'track'] for p in [model_state.radar_system.track_object] if model_state.radar_system.tracking], columns=projectile_columns)
    ])
    time_box.value = f'{model_state.time:.2f}'
    dfstream.send(all_projectiles)
    if model_state.simulation_complete:
        live_player.stop()


def reset_model():
    model_state.finalize()
    global configuration 
    configuration = json.load(open(configuration_file))
    try:
        model_state.initialize(str(configuration_file), configuration['seed'])
    except TypeError:
        model_state.initialize(str(configuration_file))

    all_projectiles = pd.concat([
        pd.DataFrame([[*p.position, *p.velocity, 'threat'] for p in model_state.projectiles], columns=projectile_columns),
        pd.DataFrame([[*p.position, *p.velocity, 'interceptor'] for p in model_state.weapons_system.interceptors], columns=projectile_columns),
        pd.DataFrame([[*p.position, *p.velocity, 'track'] for p in [model_state.radar_system.track_object] if model_state.radar_system.tracking], columns=projectile_columns)
    ])
    time_box.value = f'{model_state.time:.2f}'
    dfstream.send(all_projectiles)
    scenario_dfstream.send((model_state.radar_system, model_state.weapons_system))

dfstream = hv.streams.Pipe(data=pd.DataFrame({'x':[], 'y':[]}))
dmap = hv.DynamicMap(plot, streams=[dfstream])

scenario_dfstream = hv.streams.Pipe(data=[])
scenario_dmap = hv.DynamicMap(plot_scenario, streams=[scenario_dfstream])

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

model_button = pn.widgets.Button(name="Model", button_type="success", width=140)
model_button.js_on_click(code='window.open("./model", "_self")')
monte_carlo_button = pn.widgets.Button(name="Monte Carlo", button_type="success", width=140)
monte_carlo_button.js_on_click(code='window.open("./monte_carlo", "_self")')

links_card = pn.Card(
    pn.Row(model_button, monte_carlo_button),
    collapsible=False,
    title='Links'
)

app = pn.template.FastListTemplate(
    theme=pn.template.DarkTheme,
    header_background='#DC143C',
    accent_base_color='#708090',
    site="ruc-sci-comp-sim",
    title="Automated Defense System",
    main=[
        (dmap * scenario_dmap).opts(
            xlim=(-50, 2500),
            ylim=(-50, 1250),
            height=600,
            width=1200,
            show_grid=True
        )
    ],
    sidebar=[
        links_card,
        sim_config_card,
        sim_data_card
    ]
).servable()