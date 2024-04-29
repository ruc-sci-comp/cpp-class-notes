import panel as pn
import param
from panel.viewable import Viewer

class LivePlayer(Viewer):
    def __init__(self, play_cb, reset_cb, **params):
        self.play_button = pn.widgets.Button(name='Play', button_type="success", width=140)
        self.reset_button = pn.widgets.Button(name='Reset', button_type="success", width=140)

        self.fps_input = pn.widgets.IntSlider(name='FPS', start=1, end=25, step=1, value=25)
        self.play_cb = play_cb
        self.reset_cb = reset_cb
        self.periodic_callback = None
        super().__init__(**params)
        self._layout = pn.Column(pn.Row(self.play_button, self.reset_button), self.fps_input)

    def __panel__(self):
        return self._layout

    def stop(self):
        self.play()
        self.periodic_callback.stop()

    @pn.depends('play_button.clicks', watch=True)
    def play(self):
        if self.periodic_callback is None or not self.periodic_callback.running:
            self.play_button.name = 'Stop'
            self.periodic_callback = pn.state.add_periodic_callback(self.play_cb, period=1000//int(self.fps_input.value))
        elif self.periodic_callback.running:
            self.play_button.name = 'Play'
            self.periodic_callback.stop()

    @pn.depends('reset_button.clicks', watch=True)
    def reset(self):
        if self.periodic_callback:
            self.play_button.name = 'Play'
            self.periodic_callback.stop()
        self.reset_cb()