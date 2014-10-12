var StaticView = require('./static').StaticView;

var icon = require('../ui').icon;

var HomePage = require('../page/home').HomePage;
var UploadPage = require('../page/upload').UploadPage;

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
                                gApplication.gotoPage(HomePage);
                            }).bind(this)
                        },
                        icon('home'), 'Home'
                        )
                  )/*,*/
                //li(
                    //button(
                        //{
                            //onclick: (function() {
                                //this.application.gotoPage(Upload);
                            //}).bind(this)
                        //},
                        //icon('data-transfer-upload'), 'Upload'
                        //)
                  /*)*/
                );
        }
    }
    );

