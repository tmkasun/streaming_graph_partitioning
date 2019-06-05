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

        this.state = { status: "Connecting . . .", statusColor: "orange", p1: [], p2: [], p3: [], p0: [] };
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
        const [first, firstPartition, second, secondPartition] = data.trim().split(' ');
        return { first, second, firstPartition, secondPartition };
    }

    addEdge(event) {

        const { data } = event;
        console.log(data);
        const edge = Graph.deserializeWSData(data);
        if (!edge.first || !edge.second)
            return;
        const { first, second, firstPartition, secondPartition } = edge;
        if (!this.cy.elements().is("[id='" + first + "']")) {
            const vertext = { data: { id: first, label: "Node " + first, partition: 'p' + firstPartition } };
            this.cy.add(vertext);
        }

        if (!this.cy.elements().is("[id='" + second + "']")) {
            const vertextTwo = { data: { id: second, label: "Node " + second, partition: 'p' + secondPartition } };
            this.cy.add(vertextTwo);
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
        this.setState({ ['p' + secondPartition]: [...this.state['p' + secondPartition], second], ['p' + firstPartition]: [...this.state['p' + firstPartition], first] })
    }

    disconnect = () => {
        this.dataWS.close();
    }
    render() {
        const { status, statusColor } = this.state;
        const layout = { name: 'cose-bilkent' };
        // const layout = {
        //     name: 'spread',
        //     minDist: 40
        // };
        const stylesheet = [

            {
                selector: 'node[partition="p0"]',
                style: {
                    'shape': 'square',
                    'background-color': 'blue'
                }
            }, {
                selector: 'node[partition="p1"]',
                style: {
                    'shape': 'triangle',
                    'background-color': 'red'
                }
            }, {
                selector: 'node[partition="p2"]',
                style: {
                    'shape': 'diamond',
                    'background-color': 'green',
                    'bottom': '20px'

                }
            }, {
                selector: 'node[partition="p3"]',
                style: {
                    'shape': 'vee',
                    'background-color': 'black',
                    'top': '10px'
                }
            },
        ];

        return (
            <React.Fragment>
                <h1 style={{ color: statusColor }}>
                    Connection Status : {status}
                </h1>
                <table style={{ width: '100%' }}>
                    <tr>
                        <th style={{ textAlign: 'left' }} >Partition 1</th>
                        <th style={{ textAlign: 'left' }}>Partition 2</th>
                        <th style={{ textAlign: 'left' }}>Partition 3</th>
                        <th style={{ textAlign: 'left' }}>Partition 4</th>
                    </tr>
                    <tr>
                        <td>{this.state.p0.length}</td>
                        <td>{this.state.p1.length}</td>
                        <td>{this.state.p2.length}</td>
                        <td>{this.state.p3.length}</td>
                    </tr>
                </table>
                {status !== "Connected" ? (<button onClick={this.reConnect}>Re-Connect</button>) : (<button onClick={this.disconnect}>Disconnect</button>)}


                <CytoscapeComponent stylesheet={stylesheet} cy={cy => this.cy = cy} elements={[]} layout={layout} style={{ width: '100%', height: '100vh' }} />
            </React.Fragment>
        );
    }
}