import QtQuick
import QtQuick3D
import QtQuick3D.Helpers
import Satellite 1.0

Item
{
    width: 800
    height: 600

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
            position: Qt.vector3d(0, 0, 600)
        }

        DirectionalLight
        {
            brightness: 1
        }

        Model
        {
            Sat {}
            scale: Qt.vector3d(50, 50, 50)
            rotation: tamariw.get_q
        }
    }
}
