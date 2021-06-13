/* eslint-disable */
import React, { useRef, useEffect, useState } from 'react';
import { Network } from 'vis-network';
import { makeStyles, createStyles } from '@material-ui/core/styles';
import Button from '@material-ui/core/Button';
import { Box } from '@material-ui/core';

const useStyles = makeStyles(() => createStyles({
  root: {
    height: '75vh',
    border: '0.5px solid #248458',
  },
}));
export default function VisNet({ nodes, edges, options }) {
  const [networkInstance, setNetworkInstance] = useState();

  const visContainer = useRef(null);

  const classes = useStyles();

  useEffect(() => {
    const data = {
      nodes: nodes.current,
      edges: edges.current,
    };
    const network = new Network(visContainer.current, data, options);
    setNetworkInstance(network);
  }, [visContainer]);

  // const addNode = () => {
  //   nodes.current.add({
  //     id: nodeId,
  //     label: `node ${nodeId}`,
  //   });
  // }
  return (
    <>
      <Box my={1}>
        <Button variant='outlined' onClick={() => networkInstance.redraw()}>
          Stabilize
        </Button>
      </Box>
      <div className={classes.root} ref={visContainer} />
    </>
  );
}
