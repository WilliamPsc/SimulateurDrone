package inet.showcases.testObstacles;

import inet.environment.common.PhysicalEnvironment;
import inet.visualizer.integrated.IntegratedVisualizer;

network VisualisationObstacles
{
    parameters:
        @display("bgb=0,0");
    submodules:
        visualizer: IntegratedVisualizer {
            parameters:
                @display("p=100,50");
        }
        physicalEnvironment: PhysicalEnvironment {
            parameters:
                @display("p=100,150");
        }
}
