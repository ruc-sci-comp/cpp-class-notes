"""Live Simulation
"""
import json
import pathlib

import holoviews as hv
import numpy as np
import pandas as pd
import panel as pn

from .components.live_player import LivePlayer
from .model import MonteCarloModel

hv.extension('bokeh')
pn.extension()


configuration_file = pathlib.Path(__file__).parents[1] / 'data' / 'config.json'
configuration = json.load(open(configuration_file))
model_state = MonteCarloModel()


def plot(data):
    return hv.Points(
        data,
        kdims=['x', 'y'],
        vdims=['vx', 'vy', 'mc', 'marker_size']
    ).opts(
        color='mc',
        cmap='spectral',
        size='marker_size',
        tools=['hover']
    ).redim.range(mc=(0, mc_count_input.value))


def plot_scenario(data):
    radar, weapons = data
    plots = []
    radar_position = radar['position']
    radar_detection_range = radar['detection_range']
    radar_plot = hv.Ellipse(*radar_position, radar_detection_range * 2.0).opts(color='lightgreen') * hv.Points([radar_position]).opts(color='lightgreen', size=15)
    plots.append(radar_plot)
    weapons_position = weapons['position']
    weapons_engage_range = weapons['engage_range']
    weapons_plot = hv.Ellipse(*weapons_position, weapons_engage_range * 2.0).opts(color='red') * hv.Points([weapons_position]).opts(color='red', size=5)
    plots.append(weapons_plot)
    return hv.Overlay(plots)

base_data = pd.DataFrame({'x':[], 'y':[], 'vx':[], 'vy':[], 'mc': pd.Series(dtype=int), 'marker_size': pd.Series(dtype=int)})
dfstream = hv.streams.Buffer(data=base_data, length=100000)
dmap = hv.DynamicMap(plot, streams=[dfstream])

scenario_dfstream = hv.streams.Pipe(data=[])
scenario_dmap = hv.DynamicMap(plot_scenario, streams=[scenario_dfstream])

success_df = pd.DataFrame(columns=['Intercepted']).rename_axis('MC')
success_tb = pn.widgets.Tabulator(success_df, page_size=20, pagination='remote', theme='fast', width=320, height=640)
success_stats = pn.widgets.StaticText(name='Stats', value='0/0 : 0%')

mc_count_input = pn.widgets.IntInput(name='Number of Monte Carlo Iterations', start=1, end=250, value=50)

MC = 0
def run_model():
    global MC
    model_state.execute_model(MC)
    projectile_state = pd.DataFrame([[*p.position, *p.velocity, MC, 1] for p in model_state.mc_data[MC]], columns=['x', 'y', 'vx', 'vy', 'mc', 'marker_size'])
    projectile_state.mc = projectile_state.mc.astype(int)
    projectile_state.marker_size = projectile_state.marker_size.astype(int)
    projectile_state.loc[projectile_state.index[-1], 'marker_size'] = 10
    try:
        dfstream.send(projectile_state)
    except Exception as e:
        pass
    success_df.loc[MC] = [projectile_state.iloc[-1].y > 0.0]
    success_tb.value = success_df
    intercepted = success_df['Intercepted'].sum()
    success_stats.value = f'{intercepted}/{MC} : {100.0*intercepted/(MC+1):.2f}%'

    MC += 1

    if MC >= mc_count_input.value:
        live_player.stop()
        MC = 0

def reset_model():
    global configuration, success_df, MC
    configuration = json.load(open(configuration_file))
    success_df = pd.DataFrame(columns=['Intercepted']).rename_axis('MC')
    success_tb.value = success_df
    success_stats.value = '0/0 : 0%'
    scenario_dfstream.send((configuration['radar'], configuration['weapons']))
    model_state.clear()
    dfstream.clear()
    MC = 0

live_player = LivePlayer(run_model, reset_model)

sim_config_card = pn.Card(
    mc_count_input,
    live_player,
    collapsible=False,
    title='Simulation Controls'
)

sim_data_card = pn.Card(
    success_stats,
    collapsible=False,
    title='Simulation Data'
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
    title="Monte Carlo - Automated Defense System",
    main=[
        (dmap * scenario_dmap).opts(
            xlim=(-50, 2500),
            ylim=(-50, 1250),
            height=600,
            width=1200,
            show_grid=True
        ),
        success_tb
    ],
    sidebar=[
        links_card,
        sim_config_card,
        sim_data_card
    ]
).servable()