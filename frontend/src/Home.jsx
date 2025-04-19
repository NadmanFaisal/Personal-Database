import './Home.css'
import { insertRequest } from '../api/api'
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
    const [outputText, setOutput] = useState([]);

    const handleEnterPress = async (e) => {
        if(e.key == 'Enter') {
            e.preventDefault()
            setOutput(outputText => [...outputText, inputText])

            try {
                const response = await insertRequest(inputText)
                setOutput(outputText => [...outputText, response])
            } catch(error) {
                console.log(error)
                const response = ('Error: ' + error.message)
                setOutput(outputText => [...outputText, response])
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
