from django_components import component
from django.template.context import Context

from web.mqttmanager_ipc import send_ipc_request
from web.settings_helper import get_setting_with_default

@component.register("nspanel_status_temperature")
class NSPanelStatusTemperature(component.Component):
    template_view = None

    def get_context_data(self, id, state, temperature):
        return {
            "id": id,
            "state": state,
            "temperature": temperature,
            "temperature_unit": "°F" if get_setting_with_default("use_fahrenheit") == "True" else "°C"
        }

    def get_template_name(self, context: Context):
            if self.template_view:
                url_name = self.template_view
            else:
                url_name = context.request.resolver_match.url_name
            if url_name == "index":
                return "nspanel_status_temperature/nspanel_status_temperature_index.html"

    def get(self, request, view, nspanel_id):
        self.template_view = view
        temperature_unit = get_setting_with_default("temperature_unit")
        panel_status = send_ipc_request(F"nspanel/{nspanel_id}/status", {"command": "get"})
        args = {
            "id": nspanel_id,
            "state": panel_status["state"],
            "temperature": panel_status["temperature"],
        }
        return self.render_to_response(kwargs=args)

    class Media:
        css = "css/main.css"