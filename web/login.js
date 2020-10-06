


function showSuccessConnexionMessage()
{
     $('#login-result').modal(
       {
         focus:true,
         show:true
       }
     );
     setTimeout(function() {
         $('#login-result').modal('hide');
     }, 3000);
}
function checkUserStatus(user)
{
    if (user) {
    // User is signed in.
    var displayName = user.displayName;
    var email = user.email;
    var emailVerified = user.emailVerified;
    var photoURL = user.photoURL;
    var isAnonymous = user.isAnonymous;
    var uid = user.uid;
    var providerData = user.providerData;
    console.log("User signed in");
    // ...
    } else {
      console.log("User signed out");
    // User is signed out.
    // ...
    }
}

function connect_to_surfyn()
{
  var email = document.getElementById("email");
  var mot_de_passe = document.getElementById("mot_de_passe");

  var email_check = /^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,4}$/i;

  var email_feedback = document.getElementById("email-feedback");
  var pass_feedback = document.getElementById("pass-feedback");
  if( email_check.test(email.value) && mot_de_passe.value != "" )
  {
      email_feedback.innerHTML = "";
      email_feedback.style.color = "";
      email.className = "form-control";

      pass_feedback.innerHTML = "";
      pass_feedback.style.color = "";
      mot_de_passe.className = "form-control";

      var firebaseConfig = {
        apiKey: "AIzaSyAWjNYovhM8IDUWL_ZGZ9jnb4i14dLQSe4",
        authDomain: "surfyn.firebaseapp.com",
        databaseURL: "https://surfyn.firebaseio.com",
        projectId: "surfyn",
        storageBucket: "surfyn.appspot.com",
        messagingSenderId: "1080663386908",
        appId: "1:1080663386908:web:d0c38731a4d55de5dea8d9",
        measurementId: "G-7X8E11EEFD"
      };
      // Initialize Firebase
      if (!firebase.apps.length)
      {
        firebase.initializeApp(firebaseConfig);
        firebase.analytics();
      }


      firebase.auth().signInWithEmailAndPassword(email.value, mot_de_passe.value).then(function(user) {
        // user signed in
        showSuccessConnexionMessage();
        email.value = "";
        mot_de_passe.value = "";
      }).catch(function(error) {
      // Handle Errors here.
      var errorCode = error.code;
      var errorMessage = error.message;
      if (errorCode === 'auth/wrong-password') {
            alert('Wrong password.');
          } else {
            alert(errorMessage);
          }
          console.log(error);

    });

    firebase.auth().onAuthStateChanged(checkUserStatus);

    $("#login").modal("hide");
  }
  else
  {
      if(email_check.test(email.value) == false)
      {
        email.className = "form-control is-invalid";
        email_feedback.innerHTML = "Merci de saisir une adresse e-mail valide!";
        email_feedback.style.color = "red";
        email_feedback.style.fontSize = "12px";
      }
      else
      {
        email_feedback.innerHTML = "";
        email_feedback.style.color = "";
        email.className = "form-control";
      }
      if( mot_de_passe.value == "")
      {
        mot_de_passe.className = "form-control is-invalid";
        pass_feedback.innerHTML = "Merci de saisir un mot de passe!";
        pass_feedback.style.color = "red";
        pass_feedback.style.fontSize = "12px";
      }
      else
      {
        pass_feedback.innerHTML = "";
        pass_feedback.style.color = "";
        mot_de_passe.className = "form-control";
      }
  }
}

function connect_to_surfyn_with_google()
{
  var firebaseConfig = {
    apiKey: "AIzaSyAWjNYovhM8IDUWL_ZGZ9jnb4i14dLQSe4",
    authDomain: "surfyn.firebaseapp.com",
    databaseURL: "https://surfyn.firebaseio.com",
    projectId: "surfyn",
    storageBucket: "surfyn.appspot.com",
    messagingSenderId: "1080663386908",
    appId: "1:1080663386908:web:d0c38731a4d55de5dea8d9",
    measurementId: "G-7X8E11EEFD"
  };
  // Initialize Firebase
  if (!firebase.apps.length)
  {
    firebase.initializeApp(firebaseConfig);
    firebase.analytics();
  }

  var provider = new firebase.auth.GoogleAuthProvider();

  provider.addScope('https://www.googleapis.com/auth/userinfo.email');
  firebase.auth().signInWithRedirect(provider);
  

}
