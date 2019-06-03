import Cytoscape from 'cytoscape';
// import CS from 'cytoscape-spread';
import COSEBilkent from 'cytoscape-cose-bilkent';
import React from 'react';
import CytoscapeComponent from 'react-cytoscapejs';

// Cytoscape.use(CS);
Cytoscape.use(COSEBilkent);

export default class Graph extends React.Component {
    constructor(pp) {
        super(pp);

        this.state = { status: "Connecting . . .", statusColor: "orange" };
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
        const edge = Graph.deserializeWSData(data);
        if (!edge.first || !edge.second)
            return;
        const { first, second } = edge;
        if (!this.cy.elements().is("[id='" + first + "']"))
            this.cy.add({ data: { id: first, label: "Node " + first } });

        if (!this.cy.elements().is("[id='" + second + "']"))
            this.cy.add({ data: { id: second, label: "Node " + second } });

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
    }
    render() {
        const { status, statusColor } = this.state;
        const layout = { name: 'cose-bilkent' };
        // const layout = {
        //     name: 'spread',
        //     minDist: 40
        // };

        return (
            <React.Fragment>
                <h1 style={{ color: statusColor }}>
                    Connection Status : {status}
                </h1>
                {status !== "Connected" && <button onClick={this.reConnect}>Re-Connect</button>}


                <CytoscapeComponent cy={cy => this.cy = cy} elements={[]} layout={layout} style={{ width: '100%', height: '100vh' }} />
            </React.Fragment>
        );
    }
}