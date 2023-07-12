// Imports and Setups
require('dotenv').config()
const express = require('express')
const { Telegraf } = require('telegraf')
const app = express()
const bot = new Telegraf(process.env.BOT_TOKEN)
const port = process.env.PORT || 3000

// Middlewares
app.use(express.urlencoded())


// Main Logic
app.post('/', (req,res) => {
	try {
        let {ir, flame, airQuality} = req.body
        let message
        if (ir == 0){
            message = "Motion Detected"
        } else if (flame == 0){
            message = "Flame Detected"
        } else {
            message = "Air Quality Decresed"
        }

        bot.telegram.sendMessage(process.env.TELEGRAM_CHANNEL_ID, `Alert: ${message} \nFlame: ${flame}, IR: ${ir} and Air Quality: ${airQuality}`)
        res.sendStatus(200)
    } catch (error) {
        res.send(error)
    }
})


// Initialize the Server and Bot

bot.launch()
.then((err,stat) => {
	if (!err) console.log('Bot is Up')
	else console.log('Error occurred while Launching the Bot')
})

app.listen(port, () => {
	console.log(`Listening on port ${port}`)
})

