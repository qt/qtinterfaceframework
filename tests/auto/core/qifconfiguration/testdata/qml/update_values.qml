import QtQml
import QtInterfaceFramework

QtObject {
    property bool triggerChange: true

    property var config1: InterfaceFrameworkConfiguration {
        name: "config1"

        simulationFile: triggerChange ? "simulationFile" : "otherFile"
        simulationDataFile: triggerChange ? "simulationDataFile" : "otherFile"
        serviceSettings: triggerChange ? {"key":"value"} : {"key2": "value2"}
        preferredBackends: triggerChange ? "*backend" : ["value1", "value2"]
        discoveryMode: triggerChange ? AbstractFeature.LoadOnlyProductionBackends : AbstractFeature.LoadOnlySimulationBackends
        serviceObject: triggerChange ? serviceObject1 : serviceObject2
        backendUpdatesEnabled: triggerChange ? false : true
        asynchronousBackendLoading: triggerChange ? true : false
    }
}
