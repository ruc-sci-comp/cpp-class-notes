import panel as pn

from .model_dashboard import app as model_app

pn.serve({
    'model': model_app
}, port=5006, show=False)
