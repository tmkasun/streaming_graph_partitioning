import React from 'react';
import { Graph } from 'react-d3-graph';


// the graph configuration, you only need to pass down properties
// that you want to override, otherwise default ones will be used
const myConfig = {
    nodeHighlightBehavior: true,
    node: {
        color: 'lightgreen',
        size: 120,
        highlightStrokeColor: 'blue'
    },
    link: {
        highlightColor: 'lightblue'
    }
};


const newConfigs = {
    "automaticRearrangeAfterDropNode": true,
    "collapsible": false,
    "directed": false,
    "focusAnimationDuration": 0.75,
    "focusZoom": 1,
    "height": 1000,
    "highlightDegree": 1,
    "highlightOpacity": 0.2,
    "linkHighlightBehavior": true,
    "maxZoom": 8,
    "minZoom": 0.1,
    "nodeHighlightBehavior": true,
    "panAndZoom": false,
    "staticGraph": false,
    "width": '100%',
    "d3": {
        "alphaTarget": 0.05,
        "gravity": -100,
        "linkLength": 100,
        "linkStrength": 1
    },
    "node": {
        "color": "#d3d3d3",
        "fontColor": "black",
        "fontSize": 12,
        "fontWeight": "normal",
        "highlightColor": "red",
        "highlightFontSize": 12,
        "highlightFontWeight": "bold",
        "highlightStrokeColor": "SAME",
        "highlightStrokeWidth": 1.5,
        "labelProperty": "name",
        "mouseCursor": "pointer",
        "opacity": 1,
        "renderLabel": true,
        "size": 450,
        "strokeColor": "none",
        "strokeWidth": 1.5,
        "svg": "",
        "symbolType": "circle"
    },
    "link": {
        "color": "#d3d3d3",
        "fontColor": "black",
        "fontSize": 8,
        "fontWeight": "normal",
        "highlightColor": "blue",
        "highlightFontSize": 8,
        "highlightFontWeight": "normal",
        "labelProperty": "label",
        "mouseCursor": "pointer",
        "opacity": 1,
        "renderLabel": false,
        "semanticStrokeWidth": false,
        "strokeWidth": 4
    }
};
// graph event callbacks
const onClickGraph = function () {
    window.alert(`Clicked the graph background`);
};

const onClickNode = function (nodeId) {
    window.alert(`Clicked node ${nodeId}`);
};


export default class StreamingGraph extends React.Component {
    constructor(pp) {
        super(pp);

        this.state = {
            status: "Connecting . . .", statusColor: "orange", nodes: [{ id: -2 }],
            links: []
        };

        this.wsEndpoint = "ws://localhost:9002";
        this.addEdge = this.addEdge.bind(this);
        this.wsClose = this.wsClose.bind(this);
        this.wsOpen = this.wsOpen.bind(this);
        this.wsError = this.wsError.bind(this);
        this.connectToWS();

    }


    connectToWS = () => {
        this.dataWS = new WebSocket(this.wsEndpoint);
        this.dataWS.onmessage = this.addEdge;
        this.dataWS.onclose = this.wsClose;
        this.dataWS.onopen = this.wsOpen;
        this.dataWS.onerror = this.wsError;

    }
    wsError() {
        this.setState({ status: "Error connecting!", statusColor: "red" });

    }
    wsOpen() {
        this.setState({ status: "Connected", statusColor: "green" });

    }
    wsClose() {
        this.setState({ status: "Closed", statusColor: "pink" });

    }

    reConnect = () => {
        this.connectToWS();
    }

    static deserializeWSData(data) {
        const [first, second] = data.trim().split(' ');
        return { first, second };
    }

    addEdge(event) {
        const { data } = event;
        console.log(data);
        const edge = StreamingGraph.deserializeWSData(data);
        if (!edge.first || !edge.second)
            return;
        const { first, second } = edge;
        const { nodes, links } = this.state;
        const newNodes = [{ id: first }, { id: second }];
        const newLink = [{ source: first, target: second }];
        this.setState({ nodes: [...nodes, ...newNodes], links: [...links, ...newLink] });

    }
    componentWillUnmount() {
        console.log("Disconnecting websocket connection . . .");
        this.dataWS.close();
    }
    render() {
        const { status, statusColor, nodes, links } = this.state;
        return (
            <React.Fragment>
                <h1 style={{ color: statusColor }}>
                    Connection Status : {status}
                </h1>
                {status !== "Connected" && <button onClick={this.reConnect}>Re-Connect</button>}


                <Graph
                    id="graph-id" // id is mandatory, if no id is defined rd3g will throw an error
                    data={{ nodes, links }}
                    config={newConfigs}
                    onClickNode={onClickNode}
                    onClickGraph={onClickGraph}
                />
            </React.Fragment>
        );
    }
}