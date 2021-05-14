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
function showSuccessSubscriptionMessage()
{
     $('#inscription-result').modal(
       {
         focus:true,
         show:true
       }
     );
     setTimeout(function() {
         $('#inscription-result').modal('hide');
     }, 3000);
}

function showSuccessAlertEmailRegisteringMessage()
{
     $('#alert-email-registering-result').modal(
       {
         focus:true,
         show:true
       }
     );
     setTimeout(function() {
         $('#alert-email-registering-result').modal('hide');
     }, 3000);
}
function showSuccessMessage(id_box, id_message, message)
{
  var box_msg = document.getElementById(id_message);
  box_msg.innerHTML = message;
   $(id_box).modal(
     {
       focus:true,
       show:true
     }
   );
   setTimeout(function() {
       $(id_box).modal('hide');
   }, 3000);
}
function showSuccessMessageAndReload(id_box, id_message, message)
{
  var box_msg = document.getElementById(id_message);
  box_msg.innerHTML = message;
   $(id_box).modal(
     {
       focus:true,
       show:true
     }
   );
   setTimeout(function() {
       $(id_box).modal('hide');
       window.location.reload();
   }, 3000);
}

function showSurfynLoginPage()
{
     $('#login').modal(
       {
         focus:true,
         show:true
       }
     );
}

function signOutUser()
{
  // User is signed out.

  var user_display_name = document.getElementById("user_display_name");
  user_display_name.innerHTML = "";
  sessionStorage.setItem("user_id", "");
  sessionStorage.setItem("user_display_name", "");
  var user_not_connected_box = document.getElementById("user_not_connected_box");
  var user_connected_box = document.getElementById("user_connected_box");
  if(user_not_connected_box)
  user_not_connected_box.style.display = "flex";
  if(user_connected_box)
  user_connected_box.style.display = "none";
}
function updateUserStatus()
{
  var user_id = sessionStorage.getItem("user_id");
  var user_connected_box = document.getElementById("user_connected_box");
  var user_not_connected_box = document.getElementById("user_not_connected_box");
  if(user_id != "")
  {
      user_connected_box.style.display = "flex";
      user_not_connected_box.style.display = "none";
      var user_display_name = document.getElementById("user_display_name");
      user_display_name.innerHTML = sessionStorage.getItem("user_display_name");
  }
  else
  {
    user_not_connected_box.style.display = "flex";
    user_connected_box.style.display = "none";
  }
}
function checkUserStatus(user)
{
    if (user) {
    // User is signed in.
    var displayName = "";
    if(user.displayName)
    displayName = user.displayName;
    else {
      displayName = user.email;
    }
    var email = user.email;
    var emailVerified = user.emailVerified;
    var photoURL = user.photoURL;
    var isAnonymous = user.isAnonymous;
    var uid = user.uid;
    var providerData = user.providerData;

    var user_display_name = document.getElementById("user_display_name");
    user_display_name.innerHTML = displayName.slice(0,1).toUpperCase();

    sessionStorage.setItem("user_id", uid);
    sessionStorage.setItem("user_display_name", user_display_name.innerHTML);
    var user_not_connected_box = document.getElementById("user_not_connected_box");
    var user_connected_box = document.getElementById("user_connected_box");
    user_not_connected_box.style.display = "none";
    user_connected_box.style.display = "flex";
    } else {
      signOutUser();
    }
}



function connect_to_surfyn()
{
  var email = document.getElementById("login_email");
  var mot_de_passe = document.getElementById("mot_de_passe");

  var email_check = /^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,4}$/i;

  var email_feedback = document.getElementById("email-feedback-login");
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
  }

  var provider = new firebase.auth.GoogleAuthProvider();
  provider.setCustomParameters({ prompt: 'select_account' });

  provider.addScope('https://www.googleapis.com/auth/userinfo.email');
  firebase.auth().signInWithRedirect(provider);
}

function connect_to_surfyn_with_facebook()
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
  }

  var provider = new firebase.auth.FacebookAuthProvider();
  provider.setCustomParameters({ prompt: 'select_account' });

  firebase.auth().signInWithRedirect(provider);


}




  var passinput = document.getElementById("mot_de_passe_inscription");
  var passcheckinput = document.getElementById("mot_de_passe_confirmation");
  var letter = document.getElementById("letter");
  var capital = document.getElementById("capital");
  var number = document.getElementById("number");
  var length = document.getElementById("length");

if(passinput)
{
  // When the user clicks on the password field, show the message box
  passinput.onfocus = function() {
    document.getElementById("passwd-restriction").style.display = "flex";
  };

  // When the user clicks outside of the password field, hide the message box
  passinput.onblur = function() {
    document.getElementById("passwd-restriction").style.display = "none";
  };

  // When the user starts to type something inside the password field
  passinput.onkeyup = function()
  {
      var isValid = true;
      // Validate lowercase letters
      var lowerCaseLetters = /[a-z]/g;
      if(passinput.value.match(lowerCaseLetters)) {
        letter.style.color="green";
      } else {
        letter.style.color="red";
        isValid = false;
      }

        // Validate capital letters
        var upperCaseLetters = /[A-Z]/g;
        if(passinput.value.match(upperCaseLetters)) {
          capital.style.color="green";
        } else {
          capital.style.color="red";
          isValid = false;
      }

        // Validate numbers
        var numbers = /[0-9]/g;
        if(passinput.value.match(numbers)) {
          number.style.color="green";
        } else {
          number.style.color="red";
          isValid = false;
      }

        // Validate length
        if(passinput.value.length >= 8) {
          length.style.color="green";
        } else {
          length.style.color="red";
          isValid = false;
      }

      if(isValid)
      {
        passinput.className = "form-control";
        var mot_de_passe_inscription_feedback = document.getElementById("mot_de_passe_inscription_feedback");
        mot_de_passe_inscription_feedback.innerHTML = "";
      }
  };
}


function isValidPasswd(passwd)
{
  var lowerCaseLetters = /[a-z]/g;
  if(!passwd.match(lowerCaseLetters))
    return false;

  var upperCaseLetters = /[A-Z]/g;
  if(!passwd.match(upperCaseLetters))
    return false;

  var numbers = /[0-9]/g;
  if(!passwd.match(numbers))
    return false;

  if(passinput.value.length < 8)
    return false;

  return true;
}
var samepasscheck = document.getElementById("same-passwd");
if(passcheckinput)
{
  passcheckinput.onfocus = function() {
    document.getElementById("passwd-restriction").style.display = "none";
    document.getElementById("passwd-match").style.display = "flex";
  };
  passcheckinput.onblur = function() {
    document.getElementById("passwd-match").style.display = "none";
  };
  passcheckinput.onkeyup = function()
  {
     if(passcheckinput.value == passinput.value ) {
        samepasscheck.style.color="green";
        passcheckinput.className = "form-control";
        var mot_de_passe_confirmation_feedback = document.getElementById("mot_de_passe_confirmation_feedback");
        mot_de_passe_confirmation_feedback.innerHTML = "";
      } else {
        samepasscheck.style.color="red";
    }
 };

}


function subscribe_to_surfyn()
{
  var email = document.getElementById("inscription_email");
  var mot_de_passe = document.getElementById("mot_de_passe_inscription");
  var mot_de_passe_confirmation = document.getElementById("mot_de_passe_confirmation");

  var email_check = /^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,4}$/i;

  var email_feedback = document.getElementById("email-feedback-inscription");

  if( email_check.test(email.value) && isValidPasswd(mot_de_passe.value) && mot_de_passe.value == mot_de_passe_confirmation.value )
  {
      email_feedback.innerHTML = "";
      email_feedback.style.color = "";
      email.className = "form-control";

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
      }


      firebase.auth().createUserWithEmailAndPassword(email.value, mot_de_passe.value)
      .then((usercredentials) => {
        // Signed in
        // ...
        var prenom = document.getElementById("inscription_prenom");
        var user = usercredentials.user;
        if(user && prenom.value)
        {
           user.updateProfile({
             displayName: prenom.value
           }).then(function() {
             console.log('username successfully updated');
           }, function(error) {
             // An error happened.
             console.log(error);
           });

           var service_endpoint = "https://surfyn.fr:7878/accountcreation";
           //var service_endpoint = "http://localhost:7878/accountcreation";

           service_endpoint +="?user_displayname="
           service_endpoint += prenom.value;
           service_endpoint +="&user_email="
           service_endpoint += email.value;
           fetch(service_endpoint, {
             method : 'POST'
           })
           .catch(function(error) {
             console.log(error);
           });
        }

        $("#inscription").modal("hide");
        showSuccessSubscriptionMessage();
      }).catch(function(error) {
      // Handle Errors here.
      var errorCode = error.code;
      var errorMessage = error.message;

      alert(errorMessage);

      console.log(error);

    });

    firebase.auth().onAuthStateChanged(checkUserStatus);


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

        if(!isValidPasswd(mot_de_passe.value))
        {
          mot_de_passe.className = "form-control is-invalid";
          var mot_de_passe_inscription_feedback = document.getElementById("mot_de_passe_inscription_feedback");
          mot_de_passe_inscription_feedback.innerHTML = "Votre mot de passe doit contenir au moins 1 lettre miniscule, une lettre majuscule, un nombre, 8 caractères";
          mot_de_passe_inscription_feedback.style.color = "red";
          mot_de_passe_inscription_feedback.style.fontSize = "12px";
        }
        else {
          mot_de_passe_confirmation.className = "form-control is-invalid";
          var mot_de_passe_confirmation_feedback = document.getElementById("mot_de_passe_confirmation_feedback");
          mot_de_passe_confirmation_feedback.innerHTML = "Vos deux mots doivent être identiques";
          mot_de_passe_confirmation_feedback.style.color = "red";
          mot_de_passe_confirmation_feedback.style.fontSize = "12px";
        }
      }

  }

}

function disconnect_and_reload()
{
  // Initialize Firebase
  if (!firebase.apps.length)
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
    firebase.initializeApp(firebaseConfig);
  }
  firebase.auth().signOut().then(() => {
    // Sign-out successful.
    signOutUser();
    document.location.reload();
    console.log("Sign out successfull");
  }).catch((error) => {
  // An error happened.
  console.log(error);
  });
}

function disconnect_and_goto_homepage()
{
  // Initialize Firebase
  if (!firebase.apps.length)
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
    firebase.initializeApp(firebaseConfig);
  }
  firebase.auth().signOut().then(() => {
    // Sign-out successful.
    signOutUser();
    document.location.href ="/";
    console.log("Sign out successfull");
  }).catch((error) => {
  // An error happened.
  console.log(error);
  });

}

function isConnectedUser()
{
  return sessionStorage["user_id"];
}
function register_alert()
{
    if(isConnectedUser())
    {
      var url = "https://surfyn.fr:7878/registeremailalert";
      //var url = "http://localhost:7878/registeremailalert";

      if(window.location.search == "")
      {
        if(buildurlparams() == true)
        {
          url += url_params;
        }
        else {
          return;
        }
      }else {
        url += window.location.search;
      }
      url +="&userid="
      url += sessionStorage["user_id"];
      fetch(url, {
        method : 'POST'
      })
      .then(function(resp) {
        showSuccessAlertEmailRegisteringMessage();
       } )
      .catch(function(error) {
        console.log(error);
      });
    }
    else {
      showSurfynLoginPage();
    }
}

function passwordResetByEmail()
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
  }


  var email = document.getElementById("emailpasswordreset");
  var email_check = /^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,4}$/i;

  var email_feedback = document.getElementById("emailpasswordreset-feedback");

  if( email_check.test(email.value) )
  {

    firebase.auth().sendPasswordResetEmail(email.value).then(function() {
      $("#passwordresetbox").modal("hide");
     showSuccessMessage("#generic-box-message-result","generic-message-result","Un email sera envoyé à votre adresse pour la réinitialisation de votre mot de passe");
    }).catch(function(error) {
     alert("Erreur lors de la reinitialisation de mot de passe");
    });
  }
  else {

    email.className = "form-control is-invalid";
    email_feedback.innerHTML = "Merci de saisir une adresse e-mail valide!";
    email_feedback.style.color = "red";
    email_feedback.style.fontSize = "12px";
  }

}
