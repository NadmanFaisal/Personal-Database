import './Home.css'
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
    const [outputText, setOutput] = useState("");

    return (
        <div className="scrollable-div">
            <TopBar />
            <div className="terminal">
                {outputText}
            </div>
            <form>
                db &gt; <input className="input-field" value={inputText} type="text" onChange={(e) => setInput(e.target.value)} onKeyDown={e=>{
                    if(e.key === "Enter") {
                        e.preventDefault()
                        setOutput(inputText)
                        setInput("")
                    }
                }}/>
            </form>
        </div>
    )
}
