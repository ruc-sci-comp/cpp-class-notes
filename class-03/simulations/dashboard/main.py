"""Basic spatial + temporal dashboard
"""
import copy
import pathlib
import subprocess

import numpy as np
import panel as pn
from bokeh.models import ColumnDataSource
from bokeh.plotting import figure


main = str(pathlib.Path(__file__).parents[1] / 'bin' / 'main.exe')

cds = ColumnDataSource(data={'x':[], 'y':[]})
cds_line = ColumnDataSource(data={'x':[], 'y':[]})
fig = figure(
    x_range=(-3.0, 3.0),
    y_range=(-3.0, 3.0),
    x_axis_label='x',
    y_axis_label='y'
)
fig_container = pn.pane.Bokeh(fig)
fig.circle(x='x', y='y', size=5.0, source=cds)
fig.line(x='x', y='y', source=cds_line)

def increment_system():
    t, x, y = state.pop(0)
    cds.data = {'x': [x], 'y': [y]}
    if trace_toggle.value:
        cds_line.stream({'x': [x], 'y': [y]}, rollover=5)
    else:
        cds_line.data = {'x': [], 'y': []}
    time_box.value = str(t)
    x_box.value = str(x)
    y_box.value = str(y)
    if not state:
        play_button.name = 'Play'
        periodic_callback.stop()
        cds.data = {'x': [], 'y': []}
        cds_line.data = {'x': [], 'y': []}

periodic_callback = None
state = None
def play(event):
    global periodic_callback, state
    if periodic_callback is None or not periodic_callback.running:
        proc = subprocess.run([main], capture_output=True)
        stdout = proc.stdout.decode()
        data = [(float(line.split()[0]), float(line.split()[1]), float(line.split()[2])) for line in stdout.split('\n')[2:] if line]
        play_button.name = 'Stop'
        cds.data = {'x': [], 'y': []}
        state = copy.deepcopy(data)
        periodic_callback = pn.state.add_periodic_callback(increment_system, period=1000//int(fps_input.value), count=len(data))
    elif periodic_callback.running:
        play_button.name = 'Play'
        periodic_callback.stop()
        cds.data = {'x': [], 'y': []}
        cds_line.data = {'x': [], 'y': []}

time_box = pn.widgets.TextInput(name='time (s)', disabled=True)
x_box = pn.widgets.TextInput(name='x', disabled=True)
y_box = pn.widgets.TextInput(name='y', disabled=True)

fps_input = pn.widgets.IntSlider(name='FPS', start=1, end=30, step=1, value=30)

play_button = pn.widgets.Button(name='Play')
play_button.on_click(play)

trace_toggle = pn.widgets.Checkbox(name='Enable Trace', value=False)

sim_data_card = pn.Card(
    time_box,
    x_box,
    y_box,
    collapsible=False,
    title='Simulation Data'
)

sim_config_card = pn.Card(
    trace_toggle,
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

* To start keep all of the default values and simply click the *Play Button*. This will run our simple model and you should see an entity moving in a circular pattern!
* Click the *Stop Button*, set "Theta Delta" to 1, set "FPS" to 10, and check the "Enable Trace" box. Play the model again, and observe how non-circular the movement is!

''')

pn.template.FastListTemplate(
    theme=pn.template.DarkTheme,
    header_background='#DC143C',
    accent_base_color='#708090',
    site='Basic Simulation',
    title='Basic Spatial + Temporal System',
    main=[
        'Plotting data moving through space and time!',
        pn.Row(fig_container, description_pane),
    ],
    sidebar=[
        sim_data_card,
        sim_config_card
    ]
).servable()
