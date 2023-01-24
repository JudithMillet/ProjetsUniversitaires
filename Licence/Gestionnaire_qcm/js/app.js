/* global M loadXApiKey updateQuizData state filterHttpResponse */ // installWebSocket

// un simple ping/pong, pour montrer comment on envoie des données JSON au serveur
// eslint-disable-next-line no-unused-vars
const postEcho = (data) => {
  const url = `${state.serverUrl}/echo`;
  const body = JSON.stringify(data);
  return fetch(url, {
    method: 'POST',
    headers: state.headers,
    body,
  })
    .then(filterHttpResponse)
    .catch(console.error);
};

// //////////////////////////////////////////////////////////////////////////////
// PROGRAMME PRINCIPAL
// //////////////////////////////////////////////////////////////////////////////

function app() {
  console.debug(`@app()`);
  // ici, on lance en parallèle plusieurs actions
  return Promise.all([loadXApiKey(), updateQuizData()]).then(() => console.debug(`@app(): OK`));
}

// pour initialiser la bibliothèque Materialize
// https://materializecss.com/auto-init.html
M.AutoInit();

// lancement de l'application
app();

// pour installer le websocket
// sendMessage = installWebSocket(console.log);
