const apiCommands = require('./api.json')
const routeMessages = require('./messages.json')

var md5 = require('md5');

const express = require('express')
const app = express()
const port = 3000

var morgan = require('morgan')
app.use(morgan('combined'))

const bodyParser = require('body-parser');
app.use(bodyParser.urlencoded({
    extended: true
}));

const low = require('lowdb')
const FileSync = require('lowdb/adapters/FileSync')

const adapter = new FileSync('data/db.json')
const db = low(adapter)
db.defaults({
        counterId: [],
        counterSecret: {},
        counterStatus: {},
        counterTimestamp: {},
        counterNumber: {},
        counterLogs: {}
    })
    .write()

var dbCall = {
    getIndexOf: (cid) => db.get('counterId').indexOf(cid),
    makeLog: (cid, command, parameters) => {
        var timestamp = new Date().toISOString();
        var log = {
            timestamp,
            cid,
            command,
            parameters
        }
        db.get('counterLogs').get(cid).push(log).write()
    }
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
        // Save Secret
        var secret = md5("default") //req.body.secret
        db.get('counterSecret').set(cid, secret).write()
        // Save Status
        db.get('counterStatus').set(cid, true).write()
        // Save Default Number
        db.get('counterNumber').set(cid, 0).write()
        // Create empty Log Array
        db.get('counterLogs').set(cid, []).write()

        // Make Log
        dbCall.makeLog(cid, 'register')

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
        // Get Number from Request parameter
        var number = parseInt(req.params.number || 1); // req.body.number
        // Update number on counter
        db.get('counterNumber').update(cid, n=> parseInt(n) + number).write()
        // Get current number
        var counter = db.get('counterNumber').get(cid).value()

        // Make Log
        dbCall.makeLog(cid, 'count', {
            number,
            counter
        })

        return res.json(routeMessages.info.counterSuccess)
    }
    next(routeMessages.error.noCID)
})

app.get('/api/set/:cid/:number?', (req, res, next) => { //GET for Debug
    var cid = req.params.cid;
    var index = dbCall.getIndexOf(cid)
    if (index >= 0) {
        // Get Number from Request parameter
        var number = parseInt(req.params.number || 0); // req.body.number
        // Set number on counter
        db.get('counterNumber').set(cid, number).write()

        // Make Log
        dbCall.makeLog(cid, 'set', {
            number
        })

        return res.json(routeMessages.info.counterSetSuccess)
    }
    next(routeMessages.error.noCID)
})

app.use(function (err, req, res, next) {
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