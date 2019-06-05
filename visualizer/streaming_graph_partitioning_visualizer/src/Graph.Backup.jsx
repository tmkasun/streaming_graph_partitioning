import Cytoscape from 'cytoscape';
// import CS from 'cytoscape-spread';
import COSEBilkent from 'cytoscape-cose-bilkent';
import markovCluster from 'cytoscape-markov-cluster';
import React from 'react';
import CytoscapeComponent from 'react-cytoscapejs';

// Cytoscape.use(CS);
Cytoscape.use(COSEBilkent);
markovCluster(Cytoscape);

export default class Graph extends React.Component {
    constructor(pp) {
        super(pp);
        const { np } = pp;
        this.state = { status: "Connecting . . .", statusColor: "orange", p1: [], p2: [], p3: [], p0: [] };
        for (let index = 0; index < np; index++) {
            this.state['p' + index] = [];
        }
        this.wsEndpoint = "ws://localhost:9002";
        this.addEdge = this.addEdge.bind(this);
        this.wsClose = this.wsClose.bind(this);
        this.wsOpen = this.wsOpen.bind(this);
        this.wsError = this.wsError.bind(this);
        this.connectToWS();
        this.Headers = [];
        this.layout = { name: 'cose-bilkent' };
        // this.layout = {
        //     name: 'spread',
        //     minDist: 40
        // };
        this.stylesheet = [
            {
                selector: 'node',
                style: {
                    'label': 'data(label)'
                }
            },
        ];
        const shapes = ['square', 'triangle', 'diamond', 'vee'];
        const shapeChars = ['■', '▼', '♦', '∨']
        const colors = ['blue', 'red', 'green', 'black'];
        for (let index = 0; index < np; index++) {
            this.Headers.push(<th key={index} style={{ textAlign: 'left', color: colors[index % colors.length] }} >Partition({shapeChars[index]}) {index + 1}</th>);
            this.stylesheet.push({
                selector: 'node[partition="p' + index + '"]',
                style: {
                    'shape': shapes[index % shapes.length],
                    'background-color': colors[index % colors.length],
                }
            })
        }
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
        const [first, firstPartition, second, secondPartition] = data.trim().split(' ');
        return { first, second, firstPartition, secondPartition };
    }

    addEdge(event) {

        const { data } = event;
        console.log(data);
        const edge = Graph.deserializeWSData(data);
        const { first, second, firstPartition, secondPartition } = edge;
        let newState = {};
        if (!first || !second)
            return;
        const samePartition = firstPartition === secondPartition;
        let firstAdded = false;
        if (!this.cy.elements().is("[id='" + first + "']")) {
            const vertext = { data: { id: first, label: "Node " + first, partition: 'p' + firstPartition } };
            this.cy.add(vertext);
            newState['p' + firstPartition] = [...this.state['p' + firstPartition], first];
            firstAdded = true;
        }

        if (!this.cy.elements().is("[id='" + second + "']")) {
            const vertextTwo = { data: { id: second, label: "Node " + second, partition: 'p' + secondPartition } };
            this.cy.add(vertextTwo);
            if (samePartition && firstAdded) {
                newState['p' + secondPartition] = [...this.state['p' + secondPartition], second, first];
            } else {
                newState['p' + secondPartition] = [...this.state['p' + secondPartition], second];
            }
        }

        this.cy.add({ data: { id: first + second, source: first, target: second } });
        // const cyLayout = this.cy.layout({
        //     name: 'spread',
        //     minDist: 40
        // });
        const cyLayout = this.cy.layout({
            name: 'cose-bilkent',
            animate: 'end',
            animationEasing: 'ease-out',
            animationDuration: 1000,
            randomize: true
        });
        cyLayout.run();
        console.log(newState);
        this.setState(newState)

    }

    disconnect = () => {
        this.dataWS.close();
    }
    render() {
        const { Headers, layout, stylesheet } = this;
        const { status, statusColor } = this.state;
        const { np } = this.props;
        let Counts = [];
        for (let index = 0; index < np; index++) {
            Counts.push(<td key={index}>{this.state['p' + index].length}</td>);
        }

        return (
            <React.Fragment>
                <h1 style={{ color: statusColor }}>
                    Connection Status : {status}
                </h1>
                <table style={{ width: '100%' }}>
                    <tr>
                        {Headers}
                    </tr>
                    <tbody>
                        <tr>
                            {Counts}
                        </tr>
                    </tbody>
                </table>
                {status !== "Connected" ? (<button onClick={this.reConnect}>Re-Connect</button>) : (<button onClick={this.disconnect}>Disconnect</button>)}

                <CytoscapeComponent stylesheet={stylesheet} cy={cy => this.cy = cy} elements={[]} layout={layout} style={{ width: '100%', height: '100vh' }} />
            </React.Fragment>
        );
    }
}