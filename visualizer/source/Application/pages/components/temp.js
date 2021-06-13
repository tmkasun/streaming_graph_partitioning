let nodes; let edges; let
  network;

// convenience method to stringify a JSON object
function toJSON(obj) {
  return JSON.stringify(obj, null, 4);
}

function addNode() {
  try {
    nodes.add({
      id: document.getElementById('node-id').value,
      label: document.getElementById('node-label').value,
    });
  } catch (err) {
    alert(err);
  }
}

function updateNode() {
  try {
    nodes.update({
      id: document.getElementById('node-id').value,
      label: document.getElementById('node-label').value,
    });
  } catch (err) {
    alert(err);
  }
}
function removeNode() {
  try {
    nodes.remove({ id: document.getElementById('node-id').value });
  } catch (err) {
    alert(err);
  }
}

function addEdge() {
  try {
    edges.add({
      id: document.getElementById('edge-id').value,
      from: document.getElementById('edge-from').value,
      to: document.getElementById('edge-to').value,
    });
  } catch (err) {
    alert(err);
  }
}
function updateEdge() {
  try {
    edges.update({
      id: document.getElementById('edge-id').value,
      from: document.getElementById('edge-from').value,
      to: document.getElementById('edge-to').value,
    });
  } catch (err) {
    alert(err);
  }
}
function removeEdge() {
  try {
    edges.remove({ id: document.getElementById('edge-id').value });
  } catch (err) {
    alert(err);
  }
}

function draw() {
  // create an array with nodes
  nodes = new vis.DataSet();
  nodes.on('*', () => {
    document.getElementById('nodes').innerText = JSON.stringify(
      nodes.get(),
      null,
      4,
    );
  });
  nodes.add([
    { id: '1', label: 'Node 1' },
    { id: '2', label: 'Node 2' },
    { id: '3', label: 'Node 3' },
    { id: '4', label: 'Node 4' },
    { id: '5', label: 'Node 5' },
  ]);

  // create an array with edges
  edges = new vis.DataSet();
  edges.on('*', () => {
    document.getElementById('edges').innerText = JSON.stringify(
      edges.get(),
      null,
      4,
    );
  });
  edges.add([
    { id: '1', from: '1', to: '2' },
    { id: '2', from: '1', to: '3' },
    { id: '3', from: '2', to: '4' },
    { id: '4', from: '2', to: '5' },
  ]);

  // create a network
  const container = document.getElementById('mynetwork');
  const data = {
    nodes,
    edges,
  };
  const options = {};
  network = new vis.Network(container, data, options);
}

window.addEventListener('load', () => {
  draw();
});
