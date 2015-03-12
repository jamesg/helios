// REST responses consist of the actual response as the 'data' key inside a
// JSON object.  This allows the protocol to be extended more easily at a later
// date if required.  In the case of array responses, returning a raw array is
// also a potential security risk.

var RestCollection = Backbone.Collection.extend(
        {
            parse: function(response) {
                return response.data;
            }
        }
        );

var RestModel = Backbone.Model.extend(
        {
            parse: function(response) {
                if(_.has(response, 'data'))
                    return response.data;
                else
                    return response;
            }
        }
        );

var Album = RestModel.extend(
    {
        defaults: {
            name: ''
        },
        idAttribute: 'album_id',
        validate: function() {
            var errors = {};
            if(this.get('name') == '')
                errors['name'] = 'Name is required';
            if(!_.isEmpty(errors))
                return errors;
        },
        url: function() {
            return this.isNew()?'/album':('/album/' + this.id());
        }
    }
    );

var AlbumCollection = RestCollection.extend(
    {
        model: Album,
        url: '/album',
        set: function(models) {
            console.log('set',models);
            return RestCollection.prototype.set.apply(this, arguments);
        }
    }
    );

var Photograph = RestModel.extend(
    {
        defaults: {
            title: '',
            caption: '',
            location: '',
            taken: '',
            tags: ''
        },
        idAttribute: 'photograph_id',
        validate: function() {
            var errors = {};
            if(this.get('title') == '')
                errors['title'] = 'Title is required';
            if(this.get('location') == '')
                errors['location'] = 'Location is required.';
            if(!_.isEmpty(errors))
                return errors;
        },
        url: function() {
            return this.isNew()?'/photograph':('/photograph/' + this.get('photograph_id'));
        }
    }
    );

var PhotographsInAlbum = RestCollection.extend(
    {
        initialize: function(models, options) {
            RestCollection.prototype.initialize.apply(this, options);
            this._album = options.album;
        },
        model: Photograph,
        url: function() {
            return '/album/' + this._album.get('album_id') + '/photograph';
        }
    }
    );

var PhotographsWithTag = RestCollection.extend(
    {
        initialize: function(options) {
            RestCollection.prototype.initialize.apply(this, options);
            this._tag = options.tag;
        },
        model: Photograph,
        // TODO: what about tags with spaces?
        uri: function() { return '/tag/' + this._tag + '/photograph'; }
    }
    );

