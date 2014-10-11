var StaticView = require('./static').StaticView;

var icon = require('../ui').icon;

var Home = require('../home').Home;
var Upload = require('../upload').Upload;

exports.Navigation = StaticView.extend(
    {
        tagName: 'div',
        className: 'navigation navigation-active',
        activate: function() {
            this.$el.attr('class', 'navigation navigation-active');
        },
        deactivate: function() {
            this.$el.attr('class', 'navigation navigation-inactive');
        },
        template: function() {
            h1('Helios: ', small('Photo Storage'));
            ul(
                li(
                    button(
                        {
                            onclick: (function() {
                                gApplication.gotoView(Home);
                            }).bind(this)
                        },
                        icon('home'), 'Home'
                        )
                  )/*,*/
                //li(
                    //button(
                        //{
                            //onclick: (function() {
                                //this.application.gotoView(Upload);
                            //}).bind(this)
                        //},
                        //icon('data-transfer-upload'), 'Upload'
                        //)
                  /*)*/
                );
        }
    }
    );

