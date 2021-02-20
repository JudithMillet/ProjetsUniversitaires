/* global state getQuizzes M getCurrentQuestions sendQuiz updateQuizUser deleteQuiz deleteQuestionResponse deleteQuestion changeStateQuiz postAnswers getUserResponses getUserResponseContent getUserQuiz updateQuizData */

// //////////////////////////////////////////////////////////////////////////////
// GÉNÉRAL
// //////////////////////////////////////////////////////////////////////////////

document.addEventListener('DOMContentLoaded', () => {
  // init des modals
  const elems = document.querySelectorAll('.modal');
  M.Modal.init(elems);
  // init des collapsible
  const elemCollapseExpand = document.querySelector('.collapsible.expandable');
  M.Collapsible.init(elemCollapseExpand, {
    accordion: false,
  });
  const elemCollapse = document.querySelector('#id-modal-quiz-form-list-questions');
  M.Collapsible.init(elemCollapse);
  // on regarde si on a dejà appuyé sur le bouton sur ce navigateur, sinon on fait pulse le bouton pour le mettre en valeur
  const butUsed = localStorage.getItem('wasCreateButtonUsed');
  if (!butUsed) {
    const button = document.getElementById('id-btn-add-quiz');
    button.classList.add('pulse');
    button.onclick = () => {
      button.classList.remove('pulse');
      localStorage.setItem('wasCreateButtonUsed', true);
    };
  }
  // on change l'url quand on change de tab
  const elemTab = document.querySelectorAll('header li.tab a[href]');
  elemTab.forEach((e) => {
    e.onclick = () => {
      // window.location.hash = e.getAttribute('href')
      // pour pas spam l'historique on utilise history.replaceState plutôt que location.hash
      window.history.replaceState(null, null, e.getAttribute('href'));
    };
  });
});

// //////////////////////////////////////////////////////////////////////////////
// HTML : fonctions génération de HTML à partir des données passées en paramètre
// //////////////////////////////////////////////////////////////////////////////

// génération d'une liste de quizzes avec deux boutons en bas
const htmlQuizzesList = (quizzes, curr, total) => {
  console.debug(`@htmlQuizzesList(.., ${curr}, ${total})`);

  // un élement <li></li> pour chaque quiz. Noter qu'on fixe une donnée
  // data-quizid qui sera accessible en JS via element.dataset.quizid.
  // On définit aussi .modal-trigger et data-target="id-modal-quiz-menu"
  // pour qu'une fenêtre modale soit affichée quand on clique dessus
  // VOIR https://materializecss.com/modals.html
  const quizzesList = quizzes.map(
    (q) =>
      `<li class="collection-item cyan lighten-3 animate teal lighten-4" data-quizid="${q.quiz_id}">
        <h5>${q.title}</h5>
        ${q.description} <a class="chip">${q.owner_id}</a>
      </li>`
  );

  // le bouton "<" pour revenir à la page précédente, ou rien si c'est la première page
  // on fixe une donnée data-page pour savoir où aller via JS via element.dataset.page
  const prevBtn =
    curr !== 1
      ? `<button id="id-prev-quizzes" data-page="${
          curr - 1
        }" class="btn"><i class="material-icons">navigate_before</i></button>`
      : '';

  // le bouton ">" pour aller à la page suivante, ou rien si c'est la dernière page
  const nextBtn =
    curr !== total
      ? `<button id="id-next-quizzes" data-page="${
          curr + 1
        }" class="btn"><i class="material-icons">navigate_next</i></button>`
      : '';

  // La liste complète et les deux boutons en bas
  const html = /* html */ `
  <ul class="collection">
    ${quizzesList.join('')}
  </ul>
  <div class="row">
    <div class="col s6 left-align">${prevBtn}</div>
    <div class="col s6 right-align">${nextBtn}</div>
  </div>
  `;
  return html;
};

// génération d'une card contenant le quiz selectionné de l'utilisateur
const htmlQuizUser = (quiz) => {
  // une card pour toute les questions
  const questions = quiz.questionData.map((quest) => {
    // un paragraphe pour chaque proposition
    const propositions = quest.propositions.map(
      (prop) =>
        `<p>${prop.correct ? '<i class="tiny material-icons ">check</i>' : '<i class="tiny material-icons ">clear</i>'}
        ${prop.content}
        </p>`
    );
    // on met les paragraphes de proposition dans la card de la question
    return /* html */ `<div class="card green lighten-5">
        <a data-quizid="${quiz.quiz_id}" data-questionid="${
      quest.question_id
    }" class="btn-small btn-floating tooltipped red delete-question" data-position="bottom" data-tooltip="Supprimer cette question" style="position: absolute;top:10px;right:10px;"><i class="tiny material-icons">delete</i></a>
        <div class="card-content">
          <span class="card-title">${quest.sentence}</span>
          ${propositions.join('')}
        </div>
      </div>`;
  });
  // une card pour le quiz où on regroupe les questions et on met les boutons pour manipuler le quiz
  const html = `
            <div class="card green animate cyan lighten-4">
            <div class="valign-wrapper btn-group" style="position: absolute;top:10px;right:10px;padding:0;margin:0;">
        <div class="switch etat tooltipped" data-position="bottom" data-tooltip="Changer l'état du quiz"><label><i class="small material-icons">lock_outline</i><input type="checkbox" data-quizid="${
          quiz.quiz_id
        }" ${
    quiz.open ? 'checked' : ''
  }><span class="lever"></span><i class="small material-icons">lock_open</i></label></div>
        <a data-quizid="${
          quiz.quiz_id
        }" class="btn-small btn-floating tooltipped red delete-quiz" data-position="bottom" data-tooltip="Supprimer ce quiz"><i class="tiny material-icons">delete</i></a>
        <a data-quizid="${
          quiz.quiz_id
        }" class="btn-small btn-floating tooltipped blue darken-2 modify-quiz" data-position="bottom" data-tooltip="Modifier ce quiz"><i class="tiny material-icons">edit</i></a>
        <a data-quizid="${
          quiz.quiz_id
        }" class="btn-small btn-floating tooltipped cyan darken-2 info-quiz" data-position="bottom" data-tooltip="Statistique du quiz"><i class="tiny material-icons">info</i></a>
            </div>
            <div class="card-content pb-1">
              <span class="card-title mb-0 ">Quiz n°${quiz.quiz_id}: ${quiz.title}</span>
              ${quiz.description}
              ${questions.join('')}
            </div>
            </div>
            `;
  return html;
};

// génération du contenu des réponses à un quiz
const htmlResponsesUserContent = (quiz) => {
  // pour chaque question à laquelle on a répondu
  return quiz.content
    .map((quest) => {
      // un paragraphe pour chaque proposition de la question
      const propositions = quest.propositions.map(
        (prop) => /* html */ `
      <p>
        <label>
          <input type="checkbox" ${prop.answered ? 'checked="checked"' : ''} disabled="disabled" />
          <span>${prop.content}</span>
        </label>
      </p>`
      );
      // une card pour la question où on regroupe les propositions et on met un bouton pour supprimer la question
      return /* html */ `<div class="card blue lighten-5">
              <button data-quizid="${quest.quiz_id}" data-questionid="${
        quest.question_id
      }" class="btn-small btn-floating tooltipped red delete-response" data-position="left" data-tooltip="Supprimer cette réponse" style="position: absolute;top:10px;right:10px;"><i class="tiny material-icons">delete</i></button>
                <div class="card-content">
                  <span class="card-title">${quest.sentence}</span>
                  ${propositions.join('')}
                </div>
              </div>`;
    })
    .join('');
};

// génération d'un groupe de collapsible contenant les réponses de l'utilisateur
const htmlResponsesUser = (quizzes) => {
  let html;
  // on regarde si on est connecté sinon on l'affiche
  if (quizzes) {
    // on regarde si on a répondu à un quiz sinon on l'affiche
    if (!(quizzes.length === 0)) {
      // pour tous les quizzes auxquels on a répondu
      const quizzesAff = Array.from(quizzes).map(
        (quiz) =>
          // un collapsible pour le quiz
          /* html */ `<div class="col s12 m6 l4">
                      <ul class="collapsible blue animate lighten-4 hoverable" data-quizid="${quiz.quiz_id}">
                        <li>
                          <div class="collapsible-header">Quiz n°${quiz.quiz_id} : ${quiz.title}
                          <br>${quiz.description}
                          </div>
                          <div class="collapsible-body"></div>
                        </li>
                      </ul>
                    </div>
                `
      );
      // on regroupe les quizzes auxquels on a répondu
      html = /* html */ `<div class="row">
                ${quizzesAff.join('')}
              </div>
              `;
    } else
      html = /* html */ `<div class="center-align card-panel col l4 offset-l4 m6 offset-m3 s12 grey white-text">
                <h3>Pas de quiz rempli</h3>
              </div>`;
  } else
    html = /* html */ `<div class="center-align card-panel col l4 offset-l4 m6 offset-m3 s12 grey white-text">
              <h3>Pas d'utilisateur connecté</h3>
            </div>`;
  return html;
};

// génération d'une card contenant le quiz selectionné
const htmlSelectedQuiz = (quizzes, question, quizId, isConnected) => {
  const isOpen = quizzes.find((e) => e.quiz_id === Number(quizId)).open;
  let answeredQuiz;
  if (state.user) {
    answeredQuiz = state.user.responses.find((a) => a.quiz_id === Number(quizId));
  }
  let title = '';
  let tooltip = '';
  if (!isConnected) {
    title = '<span class="card-title red-text darken-4"><b>Connectez-vous pour répondre</b></span>';
    tooltip = 'class="tooltipped" data-tooltip="Veuillez vous connectez pour répondre" data-position="right"';
  } else if (!isOpen) {
    title = '<span class="card-title red-text darken-3">&#9940; <b>Quiz fermé</b> &#9940;</span>';
    tooltip = 'class="tooltipped" data-tooltip="Le quiz est fermé" data-position="right"';
  }
  const infoQuiz = `<span class="card-title">${quizzes.find((e) => e.quiz_id === Number(quizId)).title}</span>${quizzes.find((e) => e.quiz_id === Number(quizId)).description}`;
  const html = question.map((n) => {
    const questionId = n.question_id;
    let answeredProp;
    if (answeredQuiz) {
      const answeredQuest = answeredQuiz.answers.find((quest) => quest.question_id === questionId);
      answeredProp = answeredQuest === undefined ? undefined : answeredQuest.proposition_id;
    }
    const prop = n.propositions.map((p) => {
      return `<div style="margin : 3px;">
          <p>
            <label ${tooltip}>
              <input type="radio" name="prop-${questionId}" value="prop-${
        p.proposition_id
      }" data-quizid="${quizId}" data-questionid="${questionId}" data-propositionid="${p.proposition_id}" ${
        answeredProp === p.proposition_id ? 'checked' : ''
      } ${!isConnected || !isOpen ? 'disabled' : ''}>
              <span style="color:#167683;">
                ${p.content}
              </span>
            </label>
          </p>
        </div>`;
    });

    return `<div class="card cyan animate teal lighten-5">
              <div class="card-content pb-1">
                <h6>${n.sentence}</h6>
                ${prop.join(``)}
              </div>
            </div>`;
  });

  return `<div class="row">
            <div class="col s12">
              <div class="card cyan animate teal lighten-4">
                <form id="quiz-response">
                  <div class="card-content">
                    ${title}
                    ${infoQuiz}
                    ${html.join(``)}
                  </div>
                </form>
              </div>
            </div>
          </div>`;
};

// génération d'un modal contenant les statistiques du quiz selectionné de l'utilisateur
const htmlStatsQuiz = (quiz) => {
  let htmlNbrReponse;
  // on regarde si il y a des réponses à ce quiz sinon on l'affiche
  if (quiz.nbrReponseQuiz !== 0) {
    // Pour chaque question on met le nombre de réponse à cette question dans un li-
    const htmlNbrReponseQuiz = quiz.questionData.map(
      (question) =>
        `
        <li>
          Il y a ${question.nbrReponseQuestion} ${
          question.nbrReponseQuestion !== 0 ? 'réponses' : 'réponse'
        } à la question ${question.question_id}
        </li>`
    );
    // on utilise une variable pour regarder si un utilisateur qui a répondu, n'a pas répondu à toute les question du quiz
    let someIncompleteAnswer = false;
    // Pour chaque utilisateur qui a répondu
    const htmlListUser = quiz.notes.map((user) => {
      // c'est moyen de de rexplorer le questionData mais d'un autre côté sans l'explorer
      // on se retrouverais avec beaucoup de données en double si on ne l'explorait pas
      // on regarde chaque question
      const htmlResponse = quiz.questionData.map((question) => {
        // on regarde si l'utilisateur a répondu à la question
        const response = question.propositions.find((prop) =>
          prop.answers.some((answers) => answers.user_id === user.user_id)
        );
        // on affiche en conséquence
        if (response) {
          return `<li>${
            response.correct ? '<i class="tiny material-icons ">check</i>' : '<i class="tiny material-icons ">clear</i>'
          } Question n°${question.question_id}, ${question.sentence}: ${response.content}</li>`;
        }
        return `<li><i class="tiny material-icons ">clear</i> Question n°${question.question_id}, ${question.sentence}: <i>Pas de réponse</i></li>`;
      });
      let noteTemp = '';
      // si l'utilisateur n'a pas répondu à toutes les questions on prépare l'affichage d'une note temporaire
      if (user.incompleteAnswer) {
        noteTemp = `, Note Temporaire ${user.note}/${user.nbrReponse}`;
        someIncompleteAnswer = true;
      }
      // un collapsible pour assembler les réponses de l'utilisateur, sa ou ses note(s) et la date réponse
      return `
      <li data-userid="${user.user_id}">
        <div class="collapsible-header collapse-header-stat">
        <span class="left"> Identifiant: ${user.user_id}, Note : ${user.note}/${quiz.questions_number}${noteTemp}</span>
        <span class="right"> Répondu le ${user.date.toLocaleString()}</span><br></div>
        <div class="collapsible-body">
          <div>
            Liste des réponses :
            <ol class="userResponseList">
              ${htmlResponse.join('')}
            </ol>
          </div>
        </div>
      </li>
      `;
    });
    let tempAverage = '';
    // si un des utilisateurs n'a pas répondu à toute les questions on affiche une moyenne avec les notes temporaires
    if (someIncompleteAnswer)
      tempAverage = `, en considérant les notes temporaires, la moyenne est de ${quiz.averageTemp}/${quiz.questions_number}`;
    // on regroupe les textes précédemment fait, liste du nombre de réponse et collapsible avec un élément par utilisateur qui a répondu
    htmlNbrReponse = `
    Il y a ${quiz.nbrReponseQuiz} réponses aux questions de ce quiz
    <ol class="numResponseList">
    ${htmlNbrReponseQuiz.join('')}
    </ol>
    La moyenne au quiz est de ${quiz.average}/${quiz.questions_number}${tempAverage}
    <ul class="collapsible">
        ${htmlListUser.join('')}
    </ul>
    `;
  } else htmlNbrReponse = "Il n'y a pas de réponse aux questions de ce quiz";
  // on renvoie ce qu'on a généré plus un titre
  return /* html */ `
      <h4>Statistiques du quiz n°${quiz.quiz_id}</h4>
      ${htmlNbrReponse}
  `;
};

// génère l'html d'une nouvelle proposition pour une question dans le form pour créer un quiz
const htmlPropForm = (questionId, propositionId, propositionData) => {
  // on charge des données si on en a
  return /* html */ `
  <div class="card-content proposition" data-id="${propositionId}">
    <span>
      <div class="row valign-wrapper">
      <div class="input-field col s2">
        <span>Proposition n°${propositionId} :</span>
      </div>
      <div class="input-field col s8">
        <input id="id-form-question${questionId}-proposition${propositionId}" type="text" class="validate nomProposition" required value="${
    propositionData ? propositionData.content : ''
  }">
        <label for="id-form-question${questionId}-proposition${propositionId}">Nom de la Proposition</label>
      </div>
      <div class="col s2">
        <label>
          <input name="question${questionId}" class="selectionProposition validate" type="radio" required ${
    propositionData && propositionData.correct ? 'checked' : ''
  }/>
            <span>Correcte ?</span>
          </label>
        </div>
      </div>
    </span>
  </div>
`;
};

// ////////////////////////////////////////////////////////////////////////////////////////////////////////
// FORMULAIRE : fonctions liées au fonctionnement du formulaire de création et de modification de quiz
// ////////////////////////////////////////////////////////////////////////////////////////////////////////

const questIdToDelete = [];

// Prépare l'affichage du formulaire pour créer un quiz
function creerQuiz() {
  const modal = document.getElementById('id-modal-quiz-form');
  const titleModal = document.getElementById('id-modal-quiz-title');
  const button = document.getElementById('id-modal-quiz-button-validate');
  document.getElementById('id-quiz-form-create').reset();
  titleModal.innerHTML = `Création d'un nouveau quiz`;
  button.innerHTML = 'Ajouter un quiz';
  M.Modal.getInstance(modal).open();
}

// Ajoute une proposition au formulaire de création de quiz
function addPropositionToQuiz(questionId, propositionData) {
  const list = document.querySelector(
    `#id-modal-quiz-form-list-questions li[data-question='${questionId}'] .modal-list-proposition`
  );
  // on utilise le nombre d'élément déjà présent pour connaitre l'id de notre nouvelle proposition
  const numberProp = list.childElementCount;
  const html = htmlPropForm(questionId, numberProp, propositionData);
  // on l'ajoute à la suite des autre propositions
  list.insertAdjacentHTML('beforeend', html);
}

// Supprime une question du form
function deleteQuestionForm(event) {
  const collapse = event.target.closest('li');
  // on regarde si cette question est une modification
  if (collapse.classList.contains('modification')) {
    // si c'est le cas on l'ajoute à la liste des questions à supprimer à distance
    questIdToDelete.push(Number(collapse.dataset.question));
  }
  // on détruit le tooltip pour éviter les problèmes graphiques
  M.Tooltip.getInstance(collapse.querySelector('.tooltipped')).destroy();
  // on enlève cet élément
  collapse.remove();
}

// Ajoute une question au formulaire de création de quiz
function addQuestionToQuiz(questionData) {
  const listQuest = Array.from(document.querySelectorAll('#id-modal-quiz-form-list-questions li'));
  let questionId;
  let html;
  // si on a des données pour la question on les met dans l'html et on ne met pas de bouton pour ajouter une proposition
  if (questionData) {
    questionId = questionData.question_id;
    html = /* html */ `
    <li class="modification" data-question="${questionId}">
      <div class="collapsible-header container-grid">
        <span class="item-name left-align">Question n°${questionId}</span>
        <a class="btn-small btn-floating tooltipped red delete-question item-btn" data-position="bottom" data-tooltip="Supprimer cette question"><i class="tiny material-icons">delete</i></a>
      </div>
      <div class="collapsible-body">
        <span>
          <div class="row">
            <div class="input-field">
              <input id="id-form-question${questionId}-nom" type="text" class="titre validate" value="${questionData.sentence}" required>
              <label for="id-form-question${questionId}-nom">Nom de la Question</label>
            </div>
          </div>
          <div id="id-modal-quiz-form-list-proposition-question${questionId}" class="modal-list-proposition">
          </div>
        </span>
      </div>
    </li>
    `;
  } else {
    // on cherche un numéro de question qui n'est pas utilisé
    questionId = listQuest.reduce(
      (correctQuestionId, question) =>
        Number(question.dataset.question) === correctQuestionId ? correctQuestionId + 1 : correctQuestionId,
      0
    );
    html = /* html */ `
    <li class="creation" data-question="${questionId}">
      <div class="collapsible-header container-grid">
        <div class="item-name left-align">Question n°${questionId}</div>
        <a class="btn-small btn-floating tooltipped red delete-question item-btn" data-position="bottom" data-tooltip="Supprimer cette question"><i class="tiny material-icons">delete</i></a>
      </div>
      <div class="collapsible-body">
        <span>
          <div class="row">
            <div class="input-field">
              <input id="id-form-question${questionId}-nom" type="text" class="titre validate" required>
              <label for="id-form-question${questionId}-nom">Nom de la Question</label>
            </div>
            <button class="waves-effect waves-green btn left add-proposition" type="button" data-question="${questionId}">Ajouter une Proposition</button>
          </div>
          <div id="id-modal-quiz-form-list-proposition-question${questionId}" class="modal-list-proposition">
          </div>
        </span>
      </div>
    </li>
    `;
  }
  // on cherche l'endroit où on va placer la question
  // on regarde si on a déjà des question sinon on le place dans la liste
  if (listQuest.length !== 0) {
    const firstElem = listQuest[0];
    // si on a un questionid plus grand que la première question dans la liste
    // on cherche la question qui a le plus grand questionid plus petit que le notre
    // et on le place après cet élément
    // sinon on place la question devant le premier élément
    if (Number(firstElem.dataset.question) < questionId) {
      const elemBefore = listQuest.reduce((previousQuestion, question) => {
        if (Number(question.dataset.question) < questionId) return question;
        return previousQuestion;
      }, undefined);
      elemBefore.insertAdjacentHTML('afterend', html);
    } else firstElem.insertAdjacentHTML('beforebegin', html);
  } else document.getElementById('id-modal-quiz-form-list-questions').insertAdjacentHTML('afterbegin', html);
  // si on a des données de question, on charge les propositions de ces données
  // sinon on affiche un bouton pour ajouter des propositions
  if (questionData) {
    questionData.propositions.forEach((p) => {
      addPropositionToQuiz(questionId, p);
    });
  } else {
    const buttonAddProp = document.querySelector(
      `#id-modal-quiz-form-list-questions li[data-question="${questionId}"] .add-proposition`
    );
    buttonAddProp.addEventListener('click', () => {
      addPropositionToQuiz(questionId);
    });
  }
  const buttonDelete = document.querySelector(
    `#id-modal-quiz-form-list-questions li[data-question="${questionId}"] .delete-question`
  );
  M.Tooltip.init(buttonDelete);
  buttonDelete.addEventListener('click', deleteQuestionForm);
}

// Récupère les données du formulaire de création de quiz
// Envoie la requête et affiche le résultat
async function handleQuiz() {
  // on récupère les données du quiz qu'on crée
  const form = document.querySelector('#id-modal-quiz-form form');
  const quiz = {};
  quiz.title = document.getElementById('quiz-name-create').value;
  quiz.description = document.getElementById('id-form-description-quiz').value;
  quiz.quiz_id = form.dataset.quizid ? Number(form.dataset.quizid) : undefined;
  // on charge les données communes du formulaire
  const dataQuestion = (question) => {
    const res = {};
    res.sentence = question.querySelector('.titre').value;
    res.question_id = question.dataset.question;
    res.propositions = Array.from(question.querySelectorAll('.proposition')).map((proposition) => {
      const r = {};
      r.content = proposition.querySelector('.nomProposition').value;
      r.proposition_id = proposition.dataset.id;
      r.correct = proposition.querySelector('.selectionProposition').checked;
      return r;
    });
    return res;
  };
  // si le quiz id n'est pas undefined c'est une mise à jour de quiz
  if (form.dataset.quizid) {
    quiz.quiz_id = Number(form.dataset.quizid);
    quiz.open = Boolean(form.dataset.open);
    // on regarde si on doit supprimer des questions
    if (questIdToDelete.length !== 0) {
      const result = await Promise.all(questIdToDelete.map((question_id) => deleteQuestion(quiz.quiz_id, question_id)));
      // on affiche le résultat des requêtes
      if (result.message === undefined) {
        M.toast({
          html: `Question(s) supprimée(s) avec succès`,
          classes: 'green darken-4',
        });
      } else {
        M.toast({
          html: result.message,
          classes: 'red darken-4',
        });
      }
    }

    // on charge les données du form
    const questionsModif = Array.from(document.querySelectorAll('#id-modal-quiz-form-list-questions li.modification'));
    const questionsCreate = Array.from(document.querySelectorAll('#id-modal-quiz-form-list-questions li.creation'));
    quiz.questionsToUpdate = questionsModif.map((question) => dataQuestion(question));
    quiz.questionsToCreate = questionsCreate.map((question) => dataQuestion(question));
    // on envoie les requêtes
    const data = await updateQuizUser(quiz);
    // on affiche le résultat des requêtes
    if (data.quiz_id) {
      M.toast({
        html: `Quiz n°${data.quiz_id} modifié avec succès`,
        classes: 'green darken-4',
      });
      const modal = document.getElementById('id-modal-quiz-form');
      M.Modal.getInstance(modal).close();
      document.getElementById('id-quiz-form-create').reset();
    } else {
      M.toast({
        html: data.message,
        classes: 'red darken-4',
      });
    }
  } else {
    const questions = Array.from(document.querySelectorAll('#id-modal-quiz-form-list-questions li'));
    quiz.questions = questions.map((question) => dataQuestion(question));
    // on envoie les requêtes
    const data = await sendQuiz(quiz);

    // on affiche le résultat
    if (data.quiz_id) {
      M.toast({
        html: `Quiz n°${data.quiz_id} ajouté avec succès`,
        classes: 'green darken-4',
      });
      const modal = document.getElementById('id-modal-quiz-form');
      M.Modal.getInstance(modal).close();
      document.getElementById('id-quiz-form-create').reset();
    } else {
      M.toast({
        html: data.message,
        classes: 'red darken-4',
      });
    }
  }
}

// Verifie si un des input dans le collapsible est incorrect si c'est le cas
// il le met en valeur
function verifyCollapsibles() {
  const list = document.querySelectorAll('#id-modal-quiz-form li');
  // pour chaque collapsible on regarde s'il y a une erreur
  list.forEach((li) => {
    const error = li.querySelector('input:invalid');
    const header = li.querySelector('.collapsible-header');
    if (error !== null) header.classList.add('collapse-header-error');
    else header.classList.remove('collapse-header-error');
  });
}

// Réinitialise l'html du formulaire de quiz
function resetQuiz() {
  document.getElementById('id-modal-quiz-form-list-questions').innerHTML = '';
  const form = document.querySelector('#id-modal-quiz-form form');
  delete form.dataset.open;
  delete form.dataset.quizid;
  questIdToDelete.length = 0;
}

document.getElementById('id-btn-add-quiz').addEventListener('click', () => creerQuiz());
document.getElementById('id-modal-quiz-form-add-question').addEventListener('click', () => addQuestionToQuiz());
document.getElementById('id-quiz-form-create').addEventListener(
  'submit',
  (event) => {
    handleQuiz();
    event.preventDefault();
  },
  false
);
document.getElementById('id-modal-quiz-button-validate').addEventListener('click', verifyCollapsibles, false);
document.getElementById('id-quiz-form-create').onreset = resetQuiz;

// //////////////////////////////////////////////////////////////////////////////
// RENDUS : mise en place du HTML dans le DOM et association des événements
// //////////////////////////////////////////////////////////////////////////////

// Un modal pour confirmer une action
const modalConfirm = (funcAct, strAct) => {
  const modal = document.getElementById('id-modal-confirm');
  modal.getElementsByClassName('modal-content')[0].innerHTML = `Êtes-vous sûr de vouloir ${strAct} ?`;
  document.getElementById('id-modal-confirm-oui').onclick = funcAct;
  M.Modal.getInstance(modal).open();
};

// met la liste HTML dans le DOM et associe les handlers aux événements
// eslint-disable-next-line no-unused-vars
function renderQuizzes() {
  console.debug(`@renderQuizzes()`);

  // les éléments à mettre à jour : le conteneur pour la liste des quizzes
  const usersElt = document.getElementById('id-all-quizzes-list');
  // une fenêtre modale définie dans le HTML
  // const modal = document.getElementById('id-modal-quiz-menu');

  // on appelle la fonction de généraion et on met le HTML produit dans le DOM
  usersElt.innerHTML = htmlQuizzesList(state.quizzes.results, state.quizzes.currentPage, state.quizzes.nbPages);

  // /!\ il faut que l'affectation usersElt.innerHTML = ... ait eu lieu pour
  // /!\ que prevBtn, nextBtn et quizzes en soient pas null
  // les éléments à mettre à jour : les boutons
  const prevBtn = document.getElementById('id-prev-quizzes');
  const nextBtn = document.getElementById('id-next-quizzes');
  // la liste de tous les quizzes individuels
  const quizzes = document.querySelectorAll('#id-all-quizzes-list li');

  // les handlers quand on clique sur "<" ou ">"
  function clickBtnPager() {
    // remet à jour les données de state en demandant la page
    // identifiée dans l'attribut data-page
    // noter ici le 'this' QUI FAIT AUTOMATIQUEMENT REFERENCE
    // A L'ELEMENT AUQUEL ON ATTACHE CE HANDLER
    getQuizzes(this.dataset.page);
  }
  if (prevBtn) prevBtn.onclick = clickBtnPager;
  if (nextBtn) nextBtn.onclick = clickBtnPager;

  // qd on clique sur un quiz, on change son contenu avant affichage
  // l'affichage sera automatiquement déclenché par materializecss car on
  // a défini .modal-trigger et data-target="id-modal-quiz-menu" dans le HTML
  function clickQuiz() {
    const quizId = this.dataset.quizid;
    console.debug(`@clickQuiz(${quizId})`);
    state.currentQuiz = quizId;
    // eslint-disable-next-line no-use-before-define
    renderUserSelection(quizId);
  }

  // pour chaque quiz, on lui associe son handler
  quizzes.forEach((q) => {
    q.onclick = clickQuiz;
  });
}

// Affiche les quizzes de l'utilisateur
// eslint-disable-next-line no-unused-vars
function renderUserQuiz(quizId) {
  const main = document.getElementById('id-all-quizzes-main');
  const quiz = state.user.quizzes.find((q) => q.quiz_id === Number(quizId));
  const quizzesAff = htmlQuizUser(quiz);
  main.innerHTML = quizzesAff;
  // fonction du bouton delete
  const handleDeleteQuiz = async () => {
    const data = await deleteQuiz(quizId);
    // on affiche le résultat de l'action
    if (data.quiz_id) {
      M.toast({
        html: `Quiz n°${quizId} Supprimé avec succès`,
        classes: 'green darken-4',
      });
    } else {
      M.toast({
        html: `Erreur lors de la suppression du quiz n°${quizId} <br> ${data.name}: ${data.message}`,
        classes: 'red darken-4',
      });
    }
  };
  // Icône de suppression du quiz
  main.querySelector('.delete-quiz').onclick = () => modalConfirm(handleDeleteQuiz, `supprimer le quiz n°${quizId}`);

  // Switch de changement du quiz
  function changerEtat() {
    const quizState = {};
    quizState.open = this.checked;
    quizState.title = quiz.title;
    quizState.description = quiz.description;
    // fix un poil moche donc on met un exit delay plutôt
    // M.Tooltip.getInstance(this.closest('.tooltipped')).close();
    changeStateQuiz(quizId, quizState);
  }
  main.querySelector('.etat input').onclick = changerEtat;

  // Stats quiz
  function afficherStats() {
    const modal = document.getElementById('id-modal-quiz-stat');
    modal.getElementsByClassName('modal-content')[0].innerHTML = htmlStatsQuiz(quiz);
    const collapse = modal.querySelectorAll('.collapsible');
    M.Collapsible.init(collapse);
    // on ouvre manuellement le modal pour ne pas changer le contenu alors qu'il est visible, ça clignote, c'est pas joli
    M.Modal.getInstance(modal).open();
  }
  main.querySelector('.info-quiz').onclick = afficherStats;

  // Modifier un quiz
  function modifierQuiz() {
    const modal = document.getElementById('id-modal-quiz-form');
    const titleModal = document.getElementById('id-modal-quiz-title');
    const button = document.getElementById('id-modal-quiz-button-validate');
    const desc = document.getElementById('id-form-description-quiz');
    const nameQuiz = document.getElementById('quiz-name-create');
    const form = document.querySelector('#id-modal-quiz-form form');
    // on reset pour eviter les problèmes
    document.getElementById('id-quiz-form-create').reset();
    // on charge les données dans le form
    titleModal.innerHTML = `Modification du quiz n°${quiz.quiz_id}`;
    nameQuiz.value = quiz.title;
    desc.value = quiz.description;
    button.innerHTML = 'Modifier ce quiz';
    form.dataset.quizid = quizId;
    form.dataset.open = quiz.open;
    quiz.questionData.forEach((q) => {
      addQuestionToQuiz(q);
    });
    M.updateTextFields();
    M.textareaAutoResize(desc);
    M.Modal.getInstance(modal).open();
  }
  main.querySelector('.modify-quiz').onclick = modifierQuiz;
  // Icône de suppression d'une question
  main.querySelectorAll('.delete-question').forEach((q) => {
    const func = async () => {
      const data = await deleteQuestion(q.dataset.quizid, q.dataset.questionid);
      // on affiche le résultat de l'action
      if (data.quiz_id) {
        M.toast({
          html: `Question n°${q.dataset.questionid} du Quiz n°${q.dataset.quizid} supprimé avec succès`,
          classes: 'green darken-4',
        });
        updateQuizData();
      } else {
        M.toast({
          html: `Erreur lors de la suppression du question n°${q.dataset.questionid} du Quiz n°${q.dataset.quizid} <br> ${data.name}: ${data.message}`,
          classes: 'red darken-4',
        });
      }
    };
    q.onclick = () =>
      modalConfirm(func, `supprimer la question n°${q.dataset.questionid} du quiz n°${q.dataset.quizid}`);
  });
  const elems = document.querySelectorAll('.tooltipped');
  M.Tooltip.init(elems);
}

// affichage d'un quiz pour y répondre si l'utilisateur est connecté
// eslint-disable-next-line no-unused-vars
function renderCurrentQuiz(quizId) {
  const main = document.getElementById('id-all-quizzes-main');
  main.innerHTML = htmlSelectedQuiz(state.quizzes.results, state.question, quizId, Boolean(state.user));
  const elem = main.querySelectorAll('.tooltipped');
  M.Tooltip.init(elem);

  // Gestion de réponse à un quiz
  const validerForm = document.querySelectorAll(`#quiz-response input`);
  // Quand on clique sur un bouton radio (une des propositions), on l'ajoute à nos réponses
  async function clickValider() {
    console.debug(`@clickValider()`);
    const form = {
      quizId: this.dataset.quizid,
      questionId: this.dataset.questionid,
      propositionId: this.dataset.propositionid,
    };
    const data = await postAnswers(form);
    // on affiche le résultat de l'action
    if (data.quiz_id) {
      M.toast({
        html: `Réponse à la question n°${this.dataset.questionid} du quiz n°${this.dataset.quizid} envoyée avec succès.`,
        classes: 'green darken-4',
      });
    } else {
      M.toast({
        html: `Erreur lors de l'envoi de la question n°${this.dataset.questionid} du quiz n°${this.dataset.quizid} <br> ${data.name}: ${data.message}`,
        classes: 'red darken-4',
      });
    }
  }
  validerForm.forEach((radio) => {
    radio.onclick = clickValider;
  });
}

// Vide l'affichage des quizzes selectionnés
function renderClearSelection() {
  const main = document.getElementById('id-all-quizzes-main');
  main.innerHTML = '';
}

// Choisit la version du quiz à afficher ou nettoye l'affichage
// eslint-disable-next-line no-unused-vars
function renderUserSelection(quizId) {
  console.debug('@renderUserSelection');
  // Si le quizId en paramètre n'est pas undefined
  if (quizId) {
    // Si l'utilisateur est connecté, il a créé des quizzes, et le quizId correspond à un de ses quizzes
    // On lance le rendu de son quiz
    if (state.user && state.user.quizzes && state.user.quizzes.some((q) => q.quiz_id === Number(quizId))) {
      getUserQuiz(quizId);
    } else { // Sinon on lance le rendu du quiz
      getCurrentQuestions(quizId);
    }
  } else renderClearSelection();
}

// Charge le contenu des réponses à un quiz répondu par l'utilisateur
// eslint-disable-next-line no-unused-vars
function renderUserResponsesContent(quizId) {
  const main = document.querySelector(`#id-my-answers-list .collapsible[data-quizid='${quizId}'] .collapsible-body`);
  const content = state.user.responses.find((r) => r.quiz_id === Number(quizId)).responseData;
  main.innerHTML = htmlResponsesUserContent(content);
  // on fait un forEach sur les boutons parce qu'on perd le this avec le modal confirm
  main.querySelectorAll('.delete-response').forEach((q) => {
    const func = async () => {
      const data = await deleteQuestionResponse(q.dataset.quizid, q.dataset.questionid);
      // affiche le résultat des requêtes
      if (data.quiz_id) {
        M.toast({
          html: `Réponse à la question n°${q.dataset.questionid} du quiz n°${q.dataset.quizid} Supprimé avec succès`,
          classes: 'green darken-4',
        });
        // on refresh les réponses
        await getUserResponses();
        // si il y a toujours des réponses à ce quiz, on l'affiche
        if (state.user.responses.some((r) => r.quiz_id === Number(quizId))) {
          await getUserResponseContent(data.quiz_id);
          const collapse = document.querySelector(`#id-my-answers-list .collapsible[data-quizid='${quizId}']`);
          M.Collapsible.getInstance(collapse).open();
        }
      } else {
        M.toast({
          html: `Erreur lors de la suppression de la réponse à la question n°${q.dataset.questionid} du quiz n°${q.dataset.quizid} <br> ${data.name}: ${data.message}`,
          classes: 'red darken-4',
        });
      }
    };
    // on demande une confirmation avant de supprimer
    q.onclick = () =>
      modalConfirm(func, `supprimer la question n°${q.dataset.questionid} du quiz n°${q.dataset.quizid}`);
  });
  const color = main.closest('.collapsible').getElementsByClassName('collapsible-header')[0];
  // si le quiz est répondu dans son entièreté, on met une couleur claire
  // sinon on met une couleur plus foncée
  if (content.full) {
    color.classList.remove('blue', 'darken-4', 'white-text');
    color.classList.add('blue', 'lighten-1');
  } else {
    color.classList.remove('blue', 'lighten-1');
    color.classList.add('blue', 'darken-4', 'white-text');
  }
  // init tooltip
  const elems = main.querySelectorAll('.tooltipped');
  M.Tooltip.init(elems);
}

// Affiche les réponses aux quizzes remplis par l'utilisateur
// eslint-disable-next-line no-unused-vars
function renderUserResponses() {
  const main = document.getElementById('id-my-answers-list');
  // on met en paramètre undefined si on n'est pas connecté
  const quizzesAff = htmlResponsesUser(state.user ? state.user.responses : undefined);
  main.innerHTML = quizzesAff;
  // fonction pour gérer le chargement du contenu des réponses
  function handleResponseContent() {
    let lastTime = 0;
    // pour eviter d'envoyer trop de requête on throttle
    return function returned() {
      const now = new Date();
      if (now - lastTime >= 4000) {
        lastTime = now;
        getUserResponseContent(this.dataset.quizid);
      }
    };
  }
  const elems = document.querySelectorAll('#id-my-answers-list .collapsible');
  elems.forEach((c) => {
    c.onmouseenter = handleResponseContent();
  });
  M.Collapsible.init(elems);
}

// quand on clique sur le bouton de login, il nous dit qui on est
// eslint-disable-next-line no-unused-vars
const renderUserBtn = () => {
  const formCon = document.getElementById('id-login-modal-form-connect');
  const instanceCon = M.Modal.getInstance(formCon);
  const formDis = document.getElementById('id-login-modal-disconnect');
  const instanceDis = M.Modal.getInstance(formDis);
  const greeting = document.getElementById('id-greeting-user');
  const butLogin = document.getElementById('id-but-login');
  // si on est connecté
  if (state.user) {
    console.debug('Connecté');
    M.toast({
      html: 'Connecté',
    });
    // franglais lvl 9000
    // on customifie le modal de déconnexion
    const html = `Bonjour ${state.user.firstname} ${state.user.lastname.toUpperCase()}`;
    greeting.innerHTML = html;
    // on change le lien du bouton vers le modal de déconnexion
    butLogin.setAttribute('href', '#id-login-modal-disconnect');
    // on change l'icône du bouton
    butLogin.classList.add('connnected');
    // on reset le form de connexion pour ne pas avoir les données quand on se déconnecte puis que l'on se reconnecte
    formCon.getElementsByTagName('form')[0].reset();
    if (instanceCon.isOpen) instanceCon.close();
    document.getElementById('id-btn-add-quiz').style.display = '';
  } else {
    // on change le lien du bouton vers le modal de connexion
    butLogin.setAttribute('href', '#id-login-modal-form-connect');
    // on change l'icône du bouton
    butLogin.classList.remove('connnected');
    // si le modal de déconnexion était ouvert on le ferme
    if (instanceDis.isOpen) {
      instanceDis.close();
      M.toast({
        html: 'Déconnecté',
      });
    }
    document.getElementById('id-btn-add-quiz').style.display = 'none';
  }
  // TODO On empeche pas de renvoyer la même clée éronné et on affiche pas l'erreur de connection dans le form mais j'ai pas trouvé de moyen de le faire bien
  // si on ne s'est pas connecté et que le modal de connexion est ouvert on affiche l'erreur
  if (!state.user && instanceCon.isOpen) {
    console.debug('error on login');
    M.toast({
      html: 'Connection error, x-pi-key invalid!',
      classes: 'red darken-4',
    });
  }
};
