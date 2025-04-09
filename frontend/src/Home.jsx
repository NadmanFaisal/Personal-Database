import './Home.css'
import { useState } from 'react'

export default function CommandScreen() {
    return (
        <div className="parent-container">
            <Screen />
        </div>
    )
}

function InputField() {
    const [inputText, setInput] = useState("");
    
    function handleEnter(event) {
        event.preventDefault()
        alert(inputText)
    }

    return (
        <form onSubmit={handleEnter}>
            db &gt; <input className="input-field" value={inputText} type="text" onChange={(e) => setInput(e.target.value)} />
        </form>
    )

}

function TopBar() {
    return (
        <div className="explore-tab">
        </div>
    )
}

function Screen() {
    return (
        <div className="scrollable-div">
            <TopBar />
            <InputField />
        </div>
    )
}
