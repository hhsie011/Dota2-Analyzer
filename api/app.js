var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');
var cors = require('cors');

var indexRouter = require('./routes/index');

// search feature routers
var searchHRouter = require('./routes/search/hero');
var searchAbRouter = require('./routes/search/ability');
var searchPlRouter = require('./routes/search/player');
var searchFBRouter = require('./routes/search/match-first-blood');
var searchCRRouter = require('./routes/search/match-cluster-region');
var searchMLRouter = require('./routes/search/match-most-likes');

// insert feature routers
var insertHRouter = require('./routes/insert/hero');
var insertAbRouter = require('./routes/insert/ability');
var insertPlRouter = require('./routes/insert/player');
var insertMRouter = require('./routes/insert/match');

// update feature routers
var updateHRouter = require('./routes/update/hero');
var updateAbRouter = require('./routes/update/ability');
var updatePlRouter = require('./routes/update/player');
var updateMRouter = require('./routes/update/match');

// delete feature routers
var deleteHRouter = require('./routes/delete/hero');
var deleteAbRouter = require('./routes/delete/ability');
var deletePlRouter = require('./routes/delete/player');
var deleteMRouter = require('./routes/delete/match');

// backup/import routers
var backupRouter = require('./routes/backup/common');
var importRouter = require('./routes/import/common');

var app = express();

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'jade');

app.use(cors());
app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', indexRouter);

// search feature routing
app.use('/search/hero', searchHRouter);
app.use('/search/ability', searchAbRouter);
app.use('/search/player', searchPlRouter);
app.use('/search/match/first-blood', searchFBRouter);
app.use('/search/match/cluster-region', searchCRRouter);
app.use('/search/match/most-likes', searchMLRouter);

// insert feature routing
app.use('/insert/hero', insertHRouter);
app.use('/insert/ability', insertAbRouter);
app.use('/insert/player', insertPlRouter);
app.use('/insert/match', insertMRouter);

// update feature routing
app.use('/update/hero', updateHRouter);
app.use('/update/ability', updateAbRouter);
app.use('/update/player', updatePlRouter);
app.use('/update/match', updateMRouter);

// delete feature routing
app.use('/delete/hero', deleteHRouter);
app.use('/delete/ability', deleteAbRouter);
app.use('/delete/player', deletePlRouter);
app.use('/delete/match', deleteMRouter);

// backup/import routing
app.use('/backup/common', backupRouter);
app.use('/import/common', importRouter);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  next(createError(404));
});

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

module.exports = app;
