import QtQuick
import QtQuick3D
import QtQuick3D.Helpers
import Satellite 1.0

Item
{
    anchors.fill: parent

    Satellite
    {
        id: tamariw
    }

    View3D
    {
        anchors.fill: parent

        environment: SceneEnvironment
        {
            clearColor: "lightblue"
        }

        PerspectiveCamera
        {
            position: Qt.vector3d(0, 0, 400)
        }

        DirectionalLight
        {
            brightness: 1
        }

        Model
        {
            Sat {}
            scale: Qt.vector3d(50, 50, 50)
            position: Qt.vector3d(0, 0, 0)
            rotation: tamariw.get_q
        }
    }
}
