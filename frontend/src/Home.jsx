import './Home.css'

function Screen() {
    return (
        <div className="scrollable-div">
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