/* globals renderQuizzes renderUserBtn renderUserQuiz renderUserResponses renderCurrentQuiz renderUserResponsesContent renderUserSelection */

// //////////////////////////////////////////////////////////////////////////////
// LE MODELE, a.k.a, ETAT GLOBAL
// //////////////////////////////////////////////////////////////////////////////

// un objet global pour encapsuler l'état de l'application
// on pourrait le stocker dans le LocalStorage par exemple
const state = {
  // la clef de l'utilisateur
  xApiKey: '',

  // l'URL du serveur où accéder aux données
  serverUrl: 'https://lifap5.univ-lyon1.fr',

  // la liste des quizzes
  quizzes: [],

  // question
  question: [],

  // le quiz actuellement choisi
  currentQuiz: undefined,
  // une méthode pour l'objet 'state' qui va générer les headers pour les appels à fetch
  headers() {
    const headers = new Headers();
    headers.set('X-API-KEY', this.xApiKey);
    headers.set('Accept', 'application/json');
    headers.set('Content-Type', 'application/json');
    return headers;
  },
};

// //////////////////////////////////////////////////////////////////////////////
// OUTILS génériques
// //////////////////////////////////////////////////////////////////////////////

// un filtre simple pour récupérer les réponses HTTP qui correspondent à des
// erreurs client (4xx) ou serveur (5xx)
// eslint-disable-next-line no-unused-vars
function filterHttpResponse(response) {
  return response
    .json()
    .then((data) => {
      if (response.status >= 400 && response.status < 600) {
        throw new Error(`${data.name}: ${data.message}`);
      }
      return data;
    })
    .catch((err) => console.error(`Error on json: ${err}`));
}

// //////////////////////////////////////////////////////////////////////////////
// DONNEES DES UTILISATEURS
// //////////////////////////////////////////////////////////////////////////////

// mise-à-jour asynchrone de l'état avec les informations de l'utilisateur
// l'utilisateur est identifié via sa clef X-API-KEY lue dans l'état
// eslint-disable-next-line no-unused-vars
const getUser = () => {
  console.debug(`@getUser()`);
  const url = `${state.serverUrl}/users/whoami`;
  return fetch(url, {
    method: 'GET',
    headers: state.headers(),
  })
    .then(filterHttpResponse)
    .then((data) => {
      // /!\ ICI L'ETAT EST MODIFIE /!\
      state.user = data;
      // on lance le rendu du bouton de login
      // eslint-disable-next-line no-use-before-define
      return updateQuizData();
    })
    .then(renderUserBtn);
};

// connexion avec la clée api rentrée
// eslint-disable-next-line no-unused-vars
function connexion() {
  console.debug('Debug connexion');
  state.xApiKey = document.getElementById('id-key').value;

  const stayConnected = document.getElementById('id-check').checked;
  getUser()
    .then(() => {
      if (state.user) {
        // si on reste connecté on enregistre la clée dans le localStorage
        if (stayConnected) {
          localStorage.setItem('xApiKey', state.xApiKey);
          console.debug('sauvegarde localStorage');
        }
      } else state.xApiKey = '';
      return null;
    })
    .catch((err) => console.error(`Error on connexion: ${err}`));
}

// deconnexion de l'utilisateur
// eslint-disable-next-line no-unused-vars
function deconnexion() {
  state.xApiKey = '';
  localStorage.removeItem('xApiKey');
  getUser();
  console.debug('suppression localStorage');
}

// chargement de la clé api depuis le localStorage et mise à jour des données utilisateur
// eslint-disable-next-line no-unused-vars
function loadXApiKey() {
  state.xApiKey = localStorage.getItem('xApiKey');
  getUser();
  console.debug('chargement localStorage');
}

// lien avec les modals de connexion et de deconnexion
document.getElementById('id-login-form-connect').addEventListener(
  'submit',
  (event) => {
    connexion();
    event.preventDefault();
  },
  false
);
document.getElementById('id-button-disconnect').addEventListener(
  'click',
  (event) => {
    deconnexion();
    event.preventDefault();
  },
  false
);

// //////////////////////////////////////////////////////////////////////////////
// DONNEES DES QUIZZES
// //////////////////////////////////////////////////////////////////////////////

// mise-à-jour asynchrone de l'état avec les informations de l'utilisateur
// getQuizzes télécharge la page 'p' des quizzes et la met dans l'état
// puis relance le rendu
// eslint-disable-next-line no-unused-vars
const getQuizzes = (p = 1) => {
  console.debug(`@getQuizzes(${p})`);
  const url = `${state.serverUrl}/quizzes/?page=${p}`;

  // le téléchargement est asynchrone, la màj de l'état et le rendu se fait dans le '.then'
  return fetch(url, {
    method: 'GET',
    headers: state.headers(),
  })
    .then(filterHttpResponse)
    .then((data) => {
      // /!\ ICI L'ETAT EST MODIFIE /!\
      state.quizzes = data;
      // on a mis à jour les données, on peut relancer le rendu
      // eslint-disable-next-line no-use-before-define
      return renderQuizzes();
    });
};

// Récupère les réponses d'un quiz de l'utilisateur
// eslint-disable-next-line no-unused-vars
const getQuizQuestionAnswer = (id, pid) => {
  console.debug(`@getQuizQuestionAnswer(${id},${pid})`);
  const url = `${state.serverUrl}/quizzes/${id}/questions/${pid}/answers/`;
  // le téléchargement est asynchrone, là màj de l'état et le rendu se fait dans le '.then'
  return fetch(url, {
    method: 'GET',
    headers: state.headers(),
  }).then(filterHttpResponse);
};

// mise-à-jour asynchrone de l'état avec les informations de l'utilisateur
// getUserQuizzes télécharge les quizzes de l'utilisateur et les mets dans l'état
// retourne true si réussi et false sinon
const getUserQuizzes = () => {
  console.debug(`@getUserQuizzes()`);
  if (state.user) {
    const url = `${state.serverUrl}/users/quizzes`;
    // le téléchargement est asynchrone, là màj de l'état et le rendu se fait dans le '.then'
    return fetch(url, {
      method: 'GET',
      headers: state.headers(),
    })
      .then(filterHttpResponse)
      .then((data) => {
        state.user.quizzes = data;
        return true;
      });
  }
  return false;
};

// mise-à-jour asynchrone de l'état avec les informations de l'utilisateur
// getUserQuiz télécharge le quiz de l'utilisateur en fonction de son quizId et le met dans l'état
// puis relance le rendu
// eslint-disable-next-line no-unused-vars
const getUserQuiz = (quizId) => {
  console.debug(`@getUserQuiz(${quizId})`);
  // le téléchargement est asynchrone, la màj de l'état et le rendu se fait dans le '.then'
  const monQuiz = state.user.quizzes.find((q) => q.quiz_id === Number(quizId));
  monQuiz.questionData = [];
  const answerQuiz = monQuiz.questions_ids.map((questId) => getQuizQuestionAnswer(quizId, questId));
  return Promise.all(answerQuiz).then((data) => {
    // Calcul du nombre de réponses aux questions
    data.forEach((question) => {
      question.nbrReponseQuestion = question.propositions.reduce((acc, cur) => acc + cur.answers.length, 0);
    });
    // on stock les questionData dans question
    data.forEach((question) => {
      monQuiz.questionData[question.question_id] = question;
    });
    // quiz stats
    // Calcul du nombre de réponses aux quizzes
    monQuiz.nbrReponseQuiz = monQuiz.questionData.reduce((acc, cur) => acc + cur.nbrReponseQuestion, 0);
    // on trouve l'id des utilisateurs qui ont répondu
    const idUsers = monQuiz.questionData.reduce((accQuestion, curQuestion) => {
      const currentQuestion = curQuestion.propositions.reduce((accProp, curProp) => {
        const currentProp = curProp.answers.reduce(
          (accAnsw, curAnsw) => new Set([...accAnsw, curAnsw.user_id]),
          new Set()
        );
        return new Set([...accProp, ...currentProp]);
      }, new Set());
      return new Set([...accQuestion, ...currentQuestion]);
    }, new Set());
    // on calcule les notes des personnes qui ont répondu
    monQuiz.notes = [...idUsers].map((idUser) => {
      const val = monQuiz.questionData.reduce(
        (acc, curQuestion) => {
          // on regarde si la personne a répondu
          const response = curQuestion.propositions.find((prop) =>
            prop.answers.some((answers) => answers.user_id === idUser)
          );
          // s'il a répondu on note sa réponse
          // sinon on marque qu'il n'a pas répondu à toutes les questions
          if (response) {
            // si sa réponse est correcte on augmente sa note
            if (response.correct) acc.note += 1;
            // on augmente le nombre de réponse répondue
            acc.nbrReponse += 1;
            // on regarde pour la data
            const date = new Date(response.answers.find((answers) => answers.user_id === idUser).answered_at);
            // on prend cette date comme date de quiz si on n'en a pas ou si elle est plus récent que celle qu'on avait
            if (acc.date === undefined || (acc.date && acc.date < date)) {
              acc.date = date;
            }
          } else acc.incompleteAnswer = true;
          return acc;
        },
        { note: 0, nbrReponse: 0, date: undefined, incompleteAnswer: false }
      );
      val.user_id = idUser;
      return val;
    });
    // Calcul de la moyenne
    const sum = monQuiz.notes.reduce((acc, user) => acc + user.note, 0);
    monQuiz.average = sum / monQuiz.notes.length;
    // Calcul de la moyenne avec les notes temporaires
    const sumTemp = monQuiz.notes.reduce(
      (acc, user) => acc + (user.note / user.nbrReponse) * monQuiz.questions_number,
      0
    );
    monQuiz.averageTemp = sumTemp / monQuiz.notes.length;
    return renderUserQuiz(quizId);
  });
};

// récupère le contenu des réponses d'un utilisateur
// puis relance le rendu
// eslint-disable-next-line no-unused-vars
const getUserResponseContent = (quizId) => {
  console.debug(`@getQuizQuestions(${quizId})`);
  const url = `${state.serverUrl}/quizzes/${quizId}/questions/`;
  // le téléchargement est asynchrone, là màj de l'état et le rendu se fait dans le '.then'
  return fetch(url, {
    method: 'GET',
    headers: state.headers(),
  })
    .then(filterHttpResponse)
    .then((data) => {
      const contentQuiz = state.user.responses.find((e) => e.quiz_id === Number(quizId));
      const questionContent = {};
      // variable qui contient état de remplissement du quiz
      questionContent.full = true;
      // on regarde pour chaque question nos réponses
      questionContent.content = data.reduce((result, question) => {
        // on regarde si on a répondu à cette question
        const contentQuestionQuiz = contentQuiz.answers.find((e) => e.question_id === question.question_id);
        if (contentQuestionQuiz) {
          const questionData = Object.assign(contentQuestionQuiz, question);
          // on garde l'information sur la proposition qu'on a choisi
          questionData.propositions.forEach((prop) => {
            prop.answered = prop.proposition_id === contentQuestionQuiz.proposition_id;
          });
          result.push(questionData);
        } else questionContent.full = false;
        return result;
      }, []);
      contentQuiz.responseData = questionContent;
      return renderUserResponsesContent(quizId);
    });
};

// mise-à-jour asynchrone de l'état avec les informations de l'utilisateur
// getUserResponses télécharge les réponses de l'utilisateur et les met dans l'état
// puis relance le rendu
// eslint-disable-next-line no-unused-vars
const getUserResponses = () => {
  console.debug(`@getUserResponses()`);
  if (state.user) {
    const url = `${state.serverUrl}/users/answers`;
    // le téléchargement est asynchrone, là màj de l'état et le rendu se fait dans le '.then'
    return fetch(url, {
      method: 'GET',
      headers: state.headers(),
    })
      .then(filterHttpResponse)
      .then((data) => {
        state.user.responses = data;
        return renderUserResponses();
      });
  }
  return renderUserResponses();
};

// getCurrentQuestions télécharge les questions d'un quiz (d'après son quizId) et les met dans l'état
// puis relance le rendu
// eslint-disable-next-line no-unused-vars
const getCurrentQuestions = (quizId) => {
  console.debug(`@getCurrentQuestions(${quizId})`);
  const url = `${state.serverUrl}/quizzes/${quizId}/questions`;
  return fetch(url, { method: 'GET', headers: state.headers() })
    .then(filterHttpResponse)
    .then((data) => {
      // /!\ ICI L'ETAT EST MODIFIE /!\
      state.question = data;

      // on a mis à jour les données, on peut relancer le rendu
      // eslint-disable-next-line no-use-before-define
      return renderCurrentQuiz(quizId);
    });
};

// Identifieur pour le processus
let timeout;

// met à jour toute les données locales,
// relance le rendu
// eslint-disable-next-line no-unused-vars
const updateQuizData = () => {
  // moyen satisfait, vu que quand on debounce on ne renvoie rien
  const func = () => {
    return getQuizzes(state.quizzes.currentPage)
      .then(() => Promise.all([getUserResponses(), getUserQuizzes()]))
      .then(() => renderUserSelection(state.currentQuiz));
  };
  // Interrompt le délai
  clearTimeout(timeout);
  // rajoute un délai d'une seconde pour éviter la surcharge de requètes
  timeout = setTimeout(func, 1000);
};

// //////////////////////////////////////////////////////////////////////////////
// REPONSE A UN QUIZ
// //////////////////////////////////////////////////////////////////////////////

// Envoie le formulaire form (contenant le quizId, questionId et propositionId)
// eslint-disable-next-line no-unused-vars
const postAnswers = (form) => {
  console.debug(`@postAnswers()`);
  const url = `${state.serverUrl}/quizzes/${form.quizId}/questions/${form.questionId}/answers/${form.propositionId}`;
  return fetch(url, { method: 'POST', headers: state.headers() })
    .then(filterHttpResponse)
    .then((rep) => {
      getUserResponses();
      return rep;
    });
};

// //////////////////////////////////////////////////////////////////////////////
// CREATION,MODIFICATION,SUPPRESSION D'UN QUIZ
// //////////////////////////////////////////////////////////////////////////////

// Envoie un quiz pour le creer sur le serveur
const postQuiz = (quiz) => {
  console.debug(`@postQuiz()`);
  const url = `${state.serverUrl}/quizzes/`;
  return fetch(url, {
    method: 'POST',
    headers: state.headers(),
    body: JSON.stringify(quiz),
  }).then(filterHttpResponse);
};

// Envoie la question d'un quiz
const postQuestion = (quizid, question) => {
  console.debug(`@postQuestion(${quizid},${question})`);
  const url = `${state.serverUrl}/quizzes/${quizid}/questions/`;
  return fetch(url, {
    method: 'POST',
    headers: state.headers(),
    body: JSON.stringify(question),
  }).then(filterHttpResponse);
};

// Envoie un quiz
// eslint-disable-next-line no-unused-vars
const sendQuiz = async (quiz) => {
  console.debug(`@sendQuiz()`);
  const response = await postQuiz({ title: quiz.title, description: quiz.description });
  let resultQuestion;
  // on regarde si le quiz a bien été envoyé
  if (response.quiz_id) {
    // on regarde si on a des questions à envoyer
    if (quiz.questions.length !== 0) {
      resultQuestion = await Promise.all(quiz.questions.map((question) => postQuestion(response.quiz_id, question)));
      // on regarde si on a une erreur et on renvoie l'erreur si c'est le cas
      if (resultQuestion.message) {
        updateQuizData();
        return resultQuestion;
      }
    }
    state.currentQuiz = response.quiz_id;
    updateQuizData();
    return response;
  }
  updateQuizData();
  return response;
};

// Met à jour un quiz
// eslint-disable-next-line no-unused-vars
const putQuiz = (quizData, quiz_id) => {
  console.debug(`@putQuiz()`);
  const url = `${state.serverUrl}/quizzes/${quiz_id}/`;
  return fetch(url, {
    method: 'PUT',
    headers: state.headers(),
    body: JSON.stringify(quizData),
  }).then(filterHttpResponse);
};

// Met à jour la question d'un quiz
// eslint-disable-next-line no-unused-vars
const putQuestion = (questionData, quiz_id, question_id) => {
  console.debug(`@putQuestion(${quiz_id},${question_id})`);
  const url = `${state.serverUrl}/quizzes/${quiz_id}/questions/${question_id}/`;
  return fetch(url, {
    method: 'PUT',
    headers: state.headers(),
    body: JSON.stringify(questionData),
  }).then(filterHttpResponse);
};

// eslint-disable-next-line no-unused-vars
const updateQuizUser = (quiz) => {
  // la requête pour mettre à jour le quiz
  const requestQuiz = putQuiz({ title: quiz.title, description: quiz.description, open: quiz.open }, quiz.quiz_id);
  // les requêtes pour mettre à jour les questions
  const requestQuestPut = quiz.questionsToUpdate.map((question) =>
    putQuestion(
      { sentence: question.sentence, propositions: question.propositions },
      quiz.quiz_id,
      question.question_id
    )
  );
  // les requêtes pour ajouter des questions
  const requestQuestPost = quiz.questionsToCreate.map((question) =>
    postQuestion(quiz.quiz_id, {
      sentence: question.sentence,
      question_id: question.question_id,
      propositions: question.propositions,
    })
  );
  // on envoie les requêtes
  return Promise.all([requestQuiz, ...requestQuestPut, ...requestQuestPost]).then((data) => {
    // on regarde si on a une erreur
    if (data.message) {
      return data;
    }
    updateQuizData();
    // on renvoie les données de la requête de mise à jour du quiz
    return data[0];
  });
};

// Change l'état d'un quiz ouvert ou fermé
// eslint-disable-next-line no-unused-vars
const changeStateQuiz = (quizid, body) => {
  console.debug(`@deleteQuiz(${quizid})`);
  const url = `${state.serverUrl}/quizzes/${quizid}`;
  return fetch(url, {
    method: 'PUT',
    headers: state.headers(),
    body: JSON.stringify(body),
  }).then(filterHttpResponse);
};

// Supprime un quiz
// eslint-disable-next-line no-unused-vars
const deleteQuiz = (quiz_id) => {
  console.debug(`@deleteQuiz(${quiz_id})`);
  const url = `${state.serverUrl}/quizzes/${quiz_id}`;
  return fetch(url, {
    method: 'DELETE',
    headers: state.headers(),
  })
    .then(filterHttpResponse)
    .then((data) => {
      state.currentQuiz = undefined;
      updateQuizData();
      return data;
    });
};

// Supprime une question d'un de nos quizzes
// eslint-disable-next-line no-unused-vars
const deleteQuestion = (quiz_id, questions_id) => {
  console.debug(`@deleteQuiz(${quiz_id})`);
  const url = `${state.serverUrl}/quizzes/${quiz_id}/questions/${questions_id}`;
  return fetch(url, {
    method: 'DELETE',
    headers: state.headers(),
  })
    .then(filterHttpResponse)
    .then((data) => {
      updateQuizData();
      return data;
    });
};

// Supprime une réponse à une question
// eslint-disable-next-line no-unused-vars
const deleteQuestionResponse = (quizid, questionid) => {
  console.debug(`@deleteQuestionResponse(${quizid},${questionid})`);
  const url = `${state.serverUrl}/quizzes/${quizid}/questions/${questionid}/answers`;
  return fetch(url, {
    method: 'DELETE',
    headers: state.headers(),
  })
    .then(filterHttpResponse)
    .then((data) => {
      return data;
    });
};
