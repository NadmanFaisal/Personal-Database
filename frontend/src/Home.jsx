import './Home.css'

function InputField() {
    return (
        <form>
            db &gt; <input className="input-field" type="text" />
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

export default function CommandScreen() {
    return (
        <div className="parent-container">
            <Screen />
        </div>
    )
}