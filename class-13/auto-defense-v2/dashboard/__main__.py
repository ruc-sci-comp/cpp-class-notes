import panel as pn

from .model_dashboard import app as model_app
from .monte_carlo_dashboard import app as mc_app

pn.serve({
    'model': model_app,
    'monte_carlo': mc_app
}, port=5006, show=False)
