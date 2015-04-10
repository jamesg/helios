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
        url: '/album'
    }
    );

var Tag = RestModel.extend(
    {
        defaults: {
            photograph_count: 0
        },
        idAttribute: 'tag'
    }
    );

var TagCollection = RestCollection.extend(
    {
        model: Tag,
        url: '/tag'
    }
    );

var Location = RestModel.extend(
    {
        defaults: {
            photograph_count: 0
        },
        idAttribute: 'location'
    }
    );

var LocationCollection = RestCollection.extend(
    {
        model: Location,
        url: '/location'
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

var RandomPhotograph = Photograph.extend(
    {
        url: function() {
            return this.isNew()?'/photograph/random':('/photograph/' + this.get('photograph_id'));
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

var UncategorisedPhotographs = RestCollection.extend(
    {
        model: Photograph,
        url: '/uncategorised/photograph'
    }
    );

var PhotographsWithTag = RestCollection.extend(
    {
        initialize: function(models, options) {
            RestCollection.prototype.initialize.apply(this, options);
            this._tag = options.tag;
        },
        model: Photograph,
        url: function() { return '/tag/' + this._tag + '/photograph'; }
    }
    );

var PhotographsWithLocation = RestCollection.extend(
    {
        initialize: function(models, options) {
            RestCollection.prototype.initialize.apply(this, arguments);
            this._location = options.location;
        },
        model: Photograph,
        url: function() { return '/location/' + this._location + '/photograph'; }
    }
    );

var PhotographAlbums = RestCollection.extend(
    {
        initialize: function(models, options) {
            RestCollection.prototype.initialize.apply(this, arguments);
            this._photograph = options.photograph;
        },
        model: Album,
        url: function() {
            return '/photograph/' + this._photograph.get('photograph_id') +
                '/album';
        }
    }
    );

