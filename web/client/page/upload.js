var PageView = require('../view/page').PageView;

var MessageBox = require('../view/messagebox').MessageBox;

var icon = require('../ui').icon;

exports.UploadPage = PageView.extend(
    {
        initialize: function() {
            PageView.prototype.initialize.apply(this, arguments);
            this.render();
        },
        template: function() {
            var messageBox = new MessageBox;

            var title = input({ type: 'text', name: 'title' });
            var caption = input({ type: 'text', name: 'caption' });
            var location = input({ type: 'text', name: 'location' });
            var tags = input({ type: 'text', name: 'tags', placeholder: 'Comma-separated tags' });
            var fileInput = input({ type: 'file', name: 'file' });

            var submit = function() {
                var submissionData = {
                    title: title().value,
                    caption: caption().value,
                    tags: tags().value
                };
                if(submissionData.title == '')
                {
                    messageBox.displayWarning('Title is required');
                    return false;
                }
                if(submissionData.location == '')
                {
                    messageBox.displayWarning('Location is required');
                    return false;
                }

                var photographData = new FormData(_form());

                //TODO
                //_form().disabled = true;
                messageBox.displayInformation('Uploading');
                var reqListener = function() {
                    _form.disabled = false;
                    if(this.response['error'])
                        messageBox.displayError(this.response.error);
                    else
                        messageBox.displaySuccess('Upload Complete');
                }

                var xhr = new XMLHttpRequest();
                xhr.open(
                        'post',
                        '/insert_photograph',
                        true
                        );
                xhr.onload = reqListener;
                xhr.addEventListener(
                        'progress',
                        function(event) {
                            if(event.lengthComputable)
                            {
                                var percent = (event.loaded/event.total)*100;
                                console.log('upload percentage', percent);
                            }
                        }
                        );
                xhr.send(photographData);

                return false;
            }

            var submitButton = input(
                {
                    class: 'pure-button pure-button-primary',
                    type: 'submit',
                    value: 'Upload'
                }
                );

            var inlineInput = function(input_, label_) {
                return div(
                        { class: 'pure-control-group' },
                        label({ for: input_().name }, label_),
                        input_
                        );
            }

            var _form = form(
                {
                    class: 'pure-form pure-form-aligned',
                    method: 'POST',
                    enctype: 'multipart/form-data',
                    onsubmit: submit
                },
                fieldset(
                    legend('Enter photograph details and choose a JPEG file'),
                    inlineInput(title, 'Title'),
                    inlineInput(caption, 'Caption'),
                    inlineInput(location, 'Location'),
                    inlineInput(tags, 'Tags'),
                    inlineInput(fileInput, 'File'),
                    inlineInput(submitButton, '')
                    )
                );

            return div(
                    { class: 'pure-g' },
                    div(
                        { class: 'pure-u-1-1' },
                        h2('Upload Photograph'),
                        messageBox.el,
                        _form
                       )
                    );
        }
    }
    );

