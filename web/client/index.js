var _ = require('underscore');
var Application = require('./application').Application;
var JBone = require('jbone');

//var colours = [
    ////'antiquewhite',
    //'darkolivegreen',
    //'indianred',
    ////'lightsteelblue',
    //'slategrey'
    //];

//var randomiseColour = function() {
    //var background = _.sample(colours);
    //document.body.style['background'] = background;
    //document.body.style['transition'] = 'all 2s';
//};

window.onload = function() {
    //var background = _.sample(colours);
    //document.body.style['background'] = background;
    //setInterval(randomiseColour, 12000);
    gApplication = new Application;
};

