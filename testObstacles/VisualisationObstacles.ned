package inet.showcases.testObstacles;

import inet.common.geometry.common.OsgGeographicCoordinateSystem;
import inet.environment.common.PhysicalEnvironment;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.StandardHost;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.visualizer.integrated.IntegratedVisualizer;


network VisualisationObstacles
{
    submodules:
        visualizer: IntegratedVisualizer {
            parameters:
                @display("p=100,50");
        }
        coordinateSystem: OsgGeographicCoordinateSystem {
            parameters:
                @display("p=100,150");
        }
}

network EarthVisualization2 extends VisualisationObstacles
{
    submodules:
        physicalEnvironment: PhysicalEnvironment {
            @display("p=100,246");
        }
}

network EarthVisualization3 extends VisualisationObstacles
{
    parameters:
        int numHosts;
        @display("bgb=600,600;bgi=background/green");
    submodules:
        physicalEnvironment: PhysicalEnvironment;
        Drone[numHosts]: StandardHost {
            @display("i=misc/drone");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=259,150");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=258,50");
        }
}

network EarthVisualization4 extends VisualisationObstacles
{
    parameters:
        int numHosts;
        @display("bgb=600,600;");
    submodules:
        physicalEnvironment: PhysicalEnvironment {
            @display("p=100,200");
        }
        Drone[numHosts]: StandardHost {
            @display("i=misc/drone;p=50,50");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=259,150");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=258,50");
        }
}

network EarthVisualization5 extends VisualisationObstacles
{
    parameters:
        int numHosts;
        @display("bgb=600,600;");
    submodules:
        physicalEnvironment: PhysicalEnvironment {
            @display("p=99.056,305.624");
        }
        Drone[numHosts]: StandardHost {
            @display("i=misc/drone;p=50,50");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=259,150");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=258,50");
        }
        base: StandardHost {
            @display("p=250,-300;i=device/antennatower");
        }
}
