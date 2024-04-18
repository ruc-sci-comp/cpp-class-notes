"""Live Simulation
"""
import json
import pathlib

import holoviews as hv
import numpy as np
import pandas as pd
import panel as pn

from .mc_model import MonteCarloModel

hv.extension('bokeh')
pn.extension()

configuration_file = pathlib.Path(__file__).parents[1] / 'data' / 'config.json'
configuration = json.load(open(configuration_file))
model_state = MonteCarloModel()

def plot(data):
    projectile_plot = hv.Points(data, kdims=['x', 'y'], vdims=['vx', 'vy', 'mc']).opts(color='mc', cmap='spectral').redim.range(mc=(0,mc_count_input.value))
    return projectile_plot

dfstream = hv.streams.Buffer(data=pd.DataFrame({'x':[], 'y':[], 'vx':[], 'vy':[], 'mc': []}), length=50000)
dmap = (hv.DynamicMap(plot, streams=[dfstream])).opts(
    xlim=(0,8500),
    ylim=(0,2000),
    height=600,
    width=1000,
    show_grid=True
)

periodic_callback = None

mc_count_input = pn.widgets.IntInput(name='Number of Monte Carlo Iterations', start=1, end=1000, value=50)
play_button = pn.widgets.Button(name='Play')
reset_button = pn.widgets.Button(name='Reset')
fps_input = pn.widgets.IntSlider(name='FPS', start=1, end=30, step=1, value=30)

MC = 0

def run_model():
    global MC
    model_state.execute_model(MC)
    projectile_state = pd.DataFrame([[*p.position, *p.velocity] for p in model_state.mc_data[MC]], columns=['x', 'y', 'vx', 'vy'])
    projectile_state['mc'] = MC
    dfstream.send(projectile_state)
    MC += 1
    if MC >= mc_count_input.value:
        play(None)
        MC = 0

@pn.depends(play_button, watch=True)
def play(value):
    global periodic_callback
    if periodic_callback is None or not periodic_callback.running:
        reset(None)
        play_button.name = 'Stop'
        periodic_callback = pn.state.add_periodic_callback(run_model, period=1000//int(fps_input.value), count=mc_count_input.value)
    elif periodic_callback.running:
        play_button.name = 'Play'
        periodic_callback.stop()

@pn.depends(reset_button, watch=True)
def reset(value):
    global periodic_callback
    if periodic_callback:
        periodic_callback.stop()
        periodic_callback = None
        play_button.name = 'Play'
    model_state.clear()
    dfstream.clear()
    global MC
    MC = 0

sim_config_card = pn.Card(
    mc_count_input,
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
    title="Monte Carlo - Automated Defense System",
    main=[
        dmap
    ],
    sidebar=[
        sim_config_card
    ]
).servable()