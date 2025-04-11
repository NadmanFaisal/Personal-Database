import './Home.css'
import { insertRequest, selectRequest } from './api/api.js'
import { useState } from 'react'

export default function CommandScreen() {
    return (
        <div className="parent-container">
            <Screen />
        </div>
    )
}

function TopBar() {
    return (
        <div className="explore-tab">
        </div>
    )
}

function Screen() {
    const [inputText, setInput] = useState("");
    const [outputText] = useState([]);

    const handleEnterPress = async (e) => {
        if(e.key == 'Enter') {
            e.preventDefault()
            outputText.push(inputText)

            try {
                const response = await insertRequest(inputText)
                outputText.push(response)
            } catch(error) {
                console.log(error)
                outputText.push('Error: ' + error.message)
            }

            setInput("")
        }
    }

    return (
        <div className="scrollable-div">
            <TopBar />
            <div className="terminal">
                {outputText.map((line, index) => (
                    <div key={index}>$ {line}</div>
                ))}
            </div>

            <form>
                db &gt; <input className="input-field" value={inputText} type="text" onChange={(e) => setInput(e.target.value)} onKeyDown={handleEnterPress}/>
            </form>
        </div>
    )
}
