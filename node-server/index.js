const apiCommands = require('./api.json')
const routeMessages = require('./messages.json')
var md5 = require('md5');

const express = require('express')
const bodyParser = require('body-parser');
const app = express()
const port = 3000
app.use(bodyParser.urlencoded({
    extended: true
}));

const low = require('lowdb')
const FileSync = require('lowdb/adapters/FileSync')

const adapter = new FileSync('db.json')
const db = low(adapter)
db.defaults({
        counterId: [],
        counterSecret: {},
        counterStatus: {},
        counterTimestamp: {},
        counterNumber: {}
    })
    .write()

var dbCall = {
    getIndexOf: (cid) => db.get('counterId').indexOf(cid)
}

app.get('/api/status/:cid', (req, res, next) => {
    var cid = req.params.cid;
    var index = dbCall.getIndexOf(cid)
    if (index >= 0) {
        var timestamp = db.get('counterTimestamp').get(cid).value()
        var count = db.get('counterNumber').get(cid).value()
        var retVal = {
            timestamp,
            count
        }
        return res.json(retVal)
    }
    next(routeMessages.error.noCID)
})

app.get('/api/register/:cid', (req, res) => { //GET for Debug
    // TODO: add secret of Counter
    var cid = req.params.cid;
    var index = dbCall.getIndexOf(cid)
    if (index < 0) {
        // Save CID
        db.get('counterId').push(req.params.cid).write()
        // Save Last seen
        var now = new Date().toISOString();
        db.get('counterTimestamp').set(cid, now).write()
        //Save Secret
        var secret = md5("default") //req.body.secret
        db.get('counterSecret').set(cid, secret).write()
        //Save Status
        db.get('counterStatus').set(cid, true).write()
        //Save Default Number
        db.get('counterNumber').set(cid, 0).write()
        // Return Message
        return res.json(routeMessages.info.registrationSuccess)
    }
    // Raise Warning for existing registration
    res.json(routeMessages.warning.registrationAlreadyDone)
})

app.get('/api/count/:cid/:number?', (req, res, next) => { //GET for Debug
    var cid = req.params.cid;
    var index = dbCall.getIndexOf(cid)
    if (index >= 0) {
        var number = parseInt(req.params.number || 1);
        db.get('counterNumber').update(cid, n=> parseInt(n) + number).write()
        return res.json(routeMessages.info.counterSuccess)
    }
    next(routeMessages.error.noCID)
})

app.get('/api/set/:cid/:number?', (req, res, next) => { //GET for Debug
    var cid = req.params.cid;
    var index = dbCall.getIndexOf(cid)
    if (index >= 0) {
        var number = parseInt(req.params.number || 0);
        db.get('counterNumber').set(cid, number).write()
        return res.json(routeMessages.info.counterSetSuccess)
    }
    next(routeMessages.error.noCID)
})

app.use(function (err, req, res, next) {
    console.log(arguments)
    if (!!err) {
        next(err)
    } else {
        res.json(apiCommands)
    }
})

app.use(function (err, req, res, next) {
    if (res.headersSent) {
        return next(err);
    }
    res.status(500);
    res.json({
        error: err
    })

})

app.listen(port, () => {
    console.log(`Example app listening at http://localhost:${port}`)
})