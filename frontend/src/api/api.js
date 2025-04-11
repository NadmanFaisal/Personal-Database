const BASE_URL = 'localhost:3001'

export async function insertRequest(requestBody) {
    try {
        const response = await fetch(BASE_URL + '/POST')
        const json = response.json()
        return json
    } catch(error) {
        throw new Error(error.message)
    }
}

export async function selectRequest() {
    try {
        const response = await fetch(BASE_URL + '/GET')
        const json = response.json()
        return json
    } catch(error) {
        throw new Error(error.message)
    }
}