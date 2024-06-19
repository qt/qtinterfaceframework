import QtQml
import QtInterfaceFramework

QtObject {
    property bool triggerChange: true
    property alias ignoreOverrideWarnings: config1.ignoreOverrideWarnings

    property var config1: InterfaceFrameworkConfiguration {
        id: config1
        name: "config1"

        simulationFile: triggerChange ? "simulationFile" : "otherFile"
        serviceSettings: triggerChange ? {"key":"value"} : {"key2": "value2"}
        preferredBackends: triggerChange ? "*backend" : ["value1", "value2"]
        discoveryMode: triggerChange ? AbstractFeature.LoadOnlyProductionBackends : AbstractFeature.LoadOnlySimulationBackends
        asynchronousBackendLoading: triggerChange ? false : true
    }
}
