module Styles = {
  open Css;

  let main = style([padding(px(20))]);
};

[@react.component]
let make = () => {
  React.useEffect1(
    () => {
      // TODO: handle with proper bindings
      %bs.raw
      {|
        (function init() {
          // Older browsers might not implement mediaDevices at all, so we set an empty object first
          if (navigator.mediaDevices === undefined) {
            navigator.mediaDevices = {};
          }

          // Some browsers partially implement mediaDevices. We can't just assign an object
          // with getUserMedia as it would overwrite existing properties.
          // Here, we will just add the getUserMedia property if it's missing.
          if (navigator.mediaDevices.getUserMedia === undefined) {
            navigator.mediaDevices.getUserMedia = function(constraints) {

              // First get ahold of the legacy getUserMedia, if present
              var getUserMedia = navigator.webkitGetUserMedia || navigator.mozGetUserMedia || navigator.msGetUserMedia;

              // Some browsers just don't implement it - return a rejected promise with an error
              // to keep a consistent interface
              if (!getUserMedia) {
                return Promise.reject(new Error('getUserMedia is not implemented in this browser'));
              }

              // Otherwise, wrap the call to the old navigator.getUserMedia with a Promise
              return new Promise(function(resolve, reject) {
                getUserMedia.call(navigator, constraints, resolve, reject);
              });
            }
          }

          // set up forked web audio context, for multiple browsers
          // window. is needed otherwise Safari explodes
          var audioCtx = new (window.AudioContext || window.webkitAudioContext)();

          //set up the different audio nodes we will use for the app
          var analyser = audioCtx.createAnalyser();
          analyser.minDecibels = -100;
          analyser.maxDecibels = -10;
          analyser.smoothingTimeConstant = 0.85;

          var distortion = audioCtx.createWaveShaper();
          var gainNode = audioCtx.createGain();
          var biquadFilter = audioCtx.createBiquadFilter();

          // set up canvas context for visualizer
          var canvas = document.querySelector('.visualizer');
          var canvasCtx = canvas.getContext("2d");

          //main block for doing the audio recording
          if (navigator.mediaDevices.getUserMedia) {
            console.log('getUserMedia supported.');
            var constraints = {audio: true}
            navigator.mediaDevices.getUserMedia(constraints)
                .then(
                  function(stream) {
                    console.log(stream);
                    var source = audioCtx.createMediaStreamSource(stream);
                    source.connect(distortion);
                    distortion.connect(biquadFilter);
                    biquadFilter.connect(gainNode);
                    gainNode.connect(analyser);
                    analyser.connect(audioCtx.destination);
                    visualize();
                })
                .catch( function(err) { console.log('The following gUM error occured: ' + err);})
          } else {
            console.log('getUserMedia not supported on your browser!');
          }

          function visualize() {
            var WIDTH = canvas.width;
            var HEIGHT = canvas.height;

            analyser.fftSize = 256;
            var bufferLengthAlt = analyser.frequencyBinCount;
            console.log(bufferLengthAlt);
            var dataArrayAlt = new Uint8Array(bufferLengthAlt);

            canvasCtx.clearRect(0, 0, WIDTH, HEIGHT);

            var drawAlt = function() {
              var drawVisual = requestAnimationFrame(drawAlt);

              analyser.getByteFrequencyData(dataArrayAlt);

              canvasCtx.fillStyle = '#96342b';
              canvasCtx.fillRect(0, 0, WIDTH, HEIGHT);

              var barWidth = (WIDTH / bufferLengthAlt) * 2.5;
              var barHeight;
              var x = 0;

              for(var i = 0; i < bufferLengthAlt; i++) {
                barHeight = dataArrayAlt[i];

                canvasCtx.fillStyle = '#fff';
                canvasCtx.fillRect(x,HEIGHT-barHeight/2,barWidth,barHeight/2);

                x += barWidth + 1;
              }
            };

            drawAlt();
          }

        visualize();
        })()
       |};


      Some(() => ());
    },
    [||],
  );

  <canvas className="visualizer" width="300" height="50" />;
};