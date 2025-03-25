import QtQuick
import QtQuick3D

Node {
    id: node

    // Resources
    PrincipledMaterial {
        id: icecube_black_fl_material
        objectName: "ICECube-black-fl"
        baseColor: "#ff0a0a09"
        roughness: 0.8102633953094482
        alphaMode: PrincipledMaterial.Opaque
        indexOfRefraction: 1
    }
    PrincipledMaterial {
        id: icecube_yellow_tape_material
        objectName: "ICECube-yellow-tape"
        baseColor: "#ffc9ad5b"
        roughness: 1
        alphaMode: PrincipledMaterial.Opaque
        indexOfRefraction: 1
    }
    PrincipledMaterial {
        id: icecube_solaredges_material
        objectName: "ICECube-solaredges"
        baseColor: "#ffb76639"
        roughness: 0.8102633953094482
        alphaMode: PrincipledMaterial.Opaque
        indexOfRefraction: 1
    }
    PrincipledMaterial {
        id: icecube_solar1_material
        objectName: "ICECube-solar1"
        baseColor: "#ff21184e"
        roughness: 0.8102633953094482
        alphaMode: PrincipledMaterial.Opaque
        indexOfRefraction: 1
    }
    PrincipledMaterial {
        id: icecube_silverfoil_material
        objectName: "ICECube-silverfoil"
        baseColor: "#fff7f6f4"
        roughness: 0.8102633953094482
        alphaMode: PrincipledMaterial.Opaque
        indexOfRefraction: 1
    }
    PrincipledMaterial {
        id: icecube_grey_light_material
        objectName: "ICECube-grey-light"
        baseColor: "#ff9d9c9b"
        roughness: 0.8102633953094482
        alphaMode: PrincipledMaterial.Opaque
        indexOfRefraction: 1
    }
    PrincipledMaterial {
        id: icecube_grey_dark_material
        objectName: "ICECube-grey-dark"
        baseColor: "#ff4b4b4a"
        roughness: 0.8102633953094482
        alphaMode: PrincipledMaterial.Opaque
        indexOfRefraction: 1
    }
    PrincipledMaterial {
        id: icecube_gold_material
        objectName: "ICECube-gold"
        baseColor: "#ffc3a465"
        roughness: 0.8102633953094482
        alphaMode: PrincipledMaterial.Opaque
        indexOfRefraction: 1
    }
    PrincipledMaterial {
        id: icecube_black_sm_material
        objectName: "ICECube-black-sm"
        baseColor: "#ff0a0a09"
        roughness: 0.8102633953094482
        alphaMode: PrincipledMaterial.Opaque
        indexOfRefraction: 1
    }
    PrincipledMaterial {
        id: afta_solar1_material
        objectName: "Afta-solar1"
        baseColor: "#ff3a2d7e"
        roughness: 0.8102633953094482
        alphaMode: PrincipledMaterial.Opaque
        indexOfRefraction: 1
    }
    PrincipledMaterial {
        id: default_material
        objectName: "Default"
        baseColor: "#ffc8c8c8"
        roughness: 1
        alphaMode: PrincipledMaterial.Opaque
        indexOfRefraction: 1
    }
    PrincipledMaterial {
        id: atlas_black_sm_material
        objectName: "Atlas_black-sm"
        baseColor: "#ffc8c8c8"
        roughness: 1
        alphaMode: PrincipledMaterial.Opaque
        indexOfRefraction: 1
    }
    PrincipledMaterial {
        id: afta_solar77_material
        objectName: "Afta-solar77"
        baseColor: "#ff342a7e"
        roughness: 0.8102633953094482
        alphaMode: PrincipledMaterial.Opaque
        indexOfRefraction: 1
    }
    PrincipledMaterial {
        id: afta_solar66_material
        objectName: "Afta-solar66"
        baseColor: "#ff2f2979"
        roughness: 0.8102633953094482
        alphaMode: PrincipledMaterial.Opaque
        indexOfRefraction: 1
    }
    PrincipledMaterial {
        id: afta_solar55_material
        objectName: "Afta-solar55"
        baseColor: "#ff3a277a"
        roughness: 0.8102633953094482
        alphaMode: PrincipledMaterial.Opaque
        indexOfRefraction: 1
    }
    PrincipledMaterial {
        id: afta_solar44_material
        objectName: "Afta-solar44"
        baseColor: "#ff32297b"
        roughness: 0.8102633953094482
        alphaMode: PrincipledMaterial.Opaque
        indexOfRefraction: 1
    }
    PrincipledMaterial {
        id: afta_solar33_material
        objectName: "Afta-solar33"
        baseColor: "#ff342a7e"
        roughness: 0.8102633953094482
        alphaMode: PrincipledMaterial.Opaque
        indexOfRefraction: 1
    }
    PrincipledMaterial {
        id: afta_solar22_material
        objectName: "Afta-solar22"
        baseColor: "#ff3b317d"
        roughness: 0.8102633953094482
        alphaMode: PrincipledMaterial.Opaque
        indexOfRefraction: 1
    }

    // Nodes:
    Node {
        id: pivot_Layer_0
        objectName: "Pivot-Layer_0"
        Model {
            id: layer_0
            objectName: "Layer_0"
            source: "assets/meshes/layer_0_mesh.mesh"
            materials: [
                afta_solar1_material,
                afta_solar22_material,
                afta_solar33_material,
                afta_solar44_material,
                afta_solar55_material,
                afta_solar66_material,
                afta_solar77_material,
                atlas_black_sm_material,
                default_material,
                icecube_black_fl_material,
                icecube_black_sm_material,
                icecube_gold_material,
                icecube_grey_dark_material,
                icecube_grey_light_material,
                icecube_silverfoil_material,
                icecube_solar1_material,
                icecube_solaredges_material,
                icecube_yellow_tape_material
            ]
        }
    }

    // Animations:
}
