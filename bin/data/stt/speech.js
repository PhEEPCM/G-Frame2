var fs = require('fs');
var util = require('util');
var request = require('request');
//http://stackoverflow.com/questions/19517789/how-can-i-programmatically-shutdown-a-node-program-and-restart-it
var sys = require('sys');
var exec = require('child_process').exec;
var child_process = require('child_process');
var child;
// var file = fs.readFileSync('/stt.txt', "utf8");
// console.log(file);

var wit = require('node-wit');
//var fs = require('fs');
var ACCESS_TOKEN = "WWT5VU4DO4IX6CSGIG3RYF6QHVRJFWFI";

//gets a 5second clip of audio
child = exec("rec -r 44100 -b 8 -c 1 test.wav trim 0 00:05", function (error, stdout, stderr) {
  sys.print('stdout: ' + stdout);
  sys.print('stderr: ' + stderr);
  if (error !== null) {
    console.log('exec error: ' + error);
  }
});

var clientId = 'test-app';                             // Can be anything
var clientSecret = '5b5780f512a5436bb734be5481424d55'; // API key from Azure marketplace

setInterval(function () {
  var stream = fs.createReadStream('test.wav');
  wit.captureSpeechIntent(ACCESS_TOKEN, stream, "audio/wav", function (err, res) {
      console.log("Response from Wit for audio stream: ");
      if (err) {
        console.log("Error: ", err);
      }
      var witJSON = res;
      var witText = witJSON._text;
      console.log(witJSON._text);

      var witText = witJSON._text;
      console.log('WIT TEXT ' + witText);
      if (witText != null){
        fs.writeFile("stt.txt", witText, function(err) {
          if (err) {
            console.log(err);
          }
        });
      }
      console.log(JSON.stringify(res, null, " "));
  });
  child = exec("rec -r 44100 -b 8 -c 1 test.wav trim 0 00:05", function (error, stdout, stderr) {
    sys.print('stdout: ' + stdout);
    sys.print('stderr: ' + stderr);
    if (error !== null) {
      console.log('exec error: ' + error);
    }
  });
},
10000);


// setTimeout(function() {
//   getAccessToken(clientId, clientSecret, function(err, accessToken) {
//
//     if(err) console.log(err);
//     console.log('Got access token: ' + accessToken)
//
//     speechToText('test.wav', accessToken, function(err, res) {
//       if(err) {
//         console.log('Error in Speech to Text');
//         console.log(err);
//       }
//       if (res != undefined) {
//         console.log("Res DEFINED!");
//         console.log(res.header.status);
//         var status = res.header.status;
//         if (status == "error"){
//           child_process.fork("speech.js"); // TODO: pass args...
//           process.exit(0);
//           console.log("Should have written 'stt.txt' with 'smurf'");
//           // console.log("INSIDE ERROR");
//           // fs.writeFile("stt.txt", default_text, function(err) {
//           //   if (err) {
//           //     console.log("INSIDE FS WRITE ERROR");
//           //     console.log(err);
//           //   }
//           //   else {
//           //     console.log('restarting!!!');
//               // child_process.fork("speech.js"); // TODO: pass args...
//               // process.exit(0);
//               // console.log("Should have written 'stt.txt' with 'smurf'");
//           //   }
//           // })
//         }
//         else {
//           default_text = res.results[0].lexical;
//           fs.writeFile("stt.txt", res.results[0].lexical, function(err) {
//             if (err) {
//               console.log(err);
//               console.log('restarting!!!');
//               child_process.fork("speech.js"); // TODO: pass args...
//               process.exit(0);
//             }
//             else {
//               console.log('restarting!!!');
//               child_process.fork("speech.js"); // TODO: pass args...
//               process.exit(0);
//             }
//           });
//           console.log('Confidence ' + res.results[0].confidence + ' for: "' + res.results[0].lexical + '"');
//         }
//         //console.log(res);
//       }
//       else {
//         console.log('restarting!!!');
//         child_process.fork("speech.js"); // TODO: pass args...
//         process.exit(0);
//       }
//
//     });
//   })
//
// },
// 10000);

// ==== Helpers ====

function getAccessToken(clientId, clientSecret, callback) {
  request.post({
    url: 'https://oxford-speech.cloudapp.net/token/issueToken',
    form: {
      'grant_type': 'client_credentials',
      'client_id': encodeURIComponent(clientId),
      'client_secret': encodeURIComponent(clientSecret),
      'scope': 'https://speech.platform.bing.com'
    }
  }, function(err, resp, body) {
    if(err) return callback(err);
    try {
      var accessToken = JSON.parse(body).access_token;
      if(accessToken) {
        callback(null, accessToken);
      } else {
        callback(body);
      }
    } catch(e) {
      callback(e);
    }
  });
}

function speechToText(filename, accessToken, callback) {
  fs.readFile(filename, function(err, waveData) {
    if(err) return callback(err);
    request.post({
      url: 'https://speech.platform.bing.com/recognize/query',
      qs: {
        'scenarios': 'ulm',
        'appid': 'D4D52672-91D7-4C74-8AD8-42B1D98141A5', // This magic value is required
        'locale': 'en-US',
        'device.os': 'wp7',
        'version': '3.0',
        'format': 'json',
        'requestid': '1d4b6030-9099-11e0-91e4-0800200c9a66', // can be anything
        'instanceid': '1d4b6030-9099-11e0-91e4-0800200c9a66' // can be anything
      },
      body: waveData,
      headers: {
        'Authorization': 'Bearer ' + accessToken,
        'Content-Type': 'audio/wav; samplerate=16000',
        'Content-Length' : waveData.length
      }
    }, function(err, resp, body) {
      if(err) return callback(err);
      try {
        callback(null, JSON.parse(body));
      } catch(e) {
        callback(e);
      }
    });
  });
}
