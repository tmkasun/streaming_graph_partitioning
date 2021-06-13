import React, { useState, useRef, useEffect } from 'react';
import TextField from '@material-ui/core/TextField';

import { Box, Button } from '@material-ui/core';
import { DataSet } from 'vis-data';

import VisNet from './components/VisNet';

export default function StickyFooter() {
  const [nodeId, setNodeId] = useState('');
  const [fromNodeId, setFromNodeId] = useState('');
  const [toNodeId, setToNodeId] = useState('');
  const nodes = useRef(new DataSet([]));
  const edges = useRef(new DataSet([]));

  useEffect(() => {
    fetch('http://localhost:8081/data/nodes.json').then((res) => res.json()).then((newNodes) => {
      console.log(nodes);
      nodes.current.update(newNodes);
    });
    fetch('http://localhost:8081/data/edges.json').then((res) => res.json()).then((newEdges) => {
      console.log(nodes);
      edges.current.update(newEdges);
    });
  }, []);

  const addNode = () => {
    nodes.current.add({ id: nodeId, label: `Node - ${nodeId}` });
    setNodeId('');
  };

  const addEdge = () => {
    edges.current.add(
      {
        id: `${fromNodeId}-${toNodeId}`,
        from: fromNodeId,
        to: toNodeId,
      },
    );
  };

  const removeEdge = () => {
    edges.current.remove({ id: `${fromNodeId}-${toNodeId}` });
  };
  const removeNode = () => {
    nodes.current.remove(nodeId);
    setNodeId('');
  };
  const options = {
    nodes: {
      shape: 'dot',
      size: 15,
      font: {
        size: 10,
      },
      borderWidth: 2,
      shadow: true,
    },
    edges: {
      width: 2,
      shadow: true,
    },
  };
  return (
    <>
      <Box display="flex" justifyContent="left" alignItems="center">
        <TextField
          size="small"
          margin="dense"
          id="node-id"
          label="Node ID"
          variant="outlined"
          value={nodeId}
          onKeyDown={(e) => {
            const eventObj = window.event ? window.event : e;
            if (eventObj.keyCode === 88 && eventObj.ctrlKey) {
              removeNode();
            }
            if (e.key === 'Enter') {
              addNode();
            }
          }}
          onChange={(e) => setNodeId(e.currentTarget.value)}
        />
        <Box alignItems="center" mx={1} display="flex">
          <Button
            variant="outlined"
            color="primary"
            size="small"
            onClick={addNode}
          >
            Add
          </Button>
        </Box>
        <Box alignItems="center" display="flex">

          <Button
            variant="outlined"
            color="secondary"
            size="small"
            onClick={removeNode}
          >
            Remove
          </Button>
        </Box>
      </Box>
      <Box display="flex" justifyContent="left" alignItems="center">
        <TextField
          size="small"
          margin="dense"
          id="from-node-id"
          label="From"
          variant="outlined"
          value={fromNodeId}
          onChange={(e) => setFromNodeId(e.currentTarget.value)}
        />
        <TextField
          size="small"
          margin="dense"
          id="to-node-id"
          label="To"
          variant="outlined"
          value={toNodeId}
          onChange={(e) => setToNodeId(e.currentTarget.value)}
        />
        <Box alignItems="center" mx={1} display="flex">
          <Button
            variant="outlined"
            color="primary"
            size="small"
            onClick={addEdge}
          >
            Add
          </Button>
        </Box>
        <Box alignItems="center" display="flex">

          <Button
            variant="outlined"
            color="secondary"
            size="small"
            onClick={removeEdge}
          >
            Remove
          </Button>
        </Box>
      </Box>

      <VisNet options={options} edges={edges} nodes={nodes} />
    </>
  );
}
