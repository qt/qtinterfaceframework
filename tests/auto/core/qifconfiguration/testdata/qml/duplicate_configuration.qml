import QtQml
import QtInterfaceFramework

QtObject {
    property var config1: InterfaceFrameworkConfiguration {
        name: "config1"
        simulationFile: "foo"
    }

    property var config2: InterfaceFrameworkConfiguration {
        name: "config1"
        simulationFile: "bar"
    }
}
