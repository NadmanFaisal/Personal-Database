const BASE_URL = 'http://localhost:3001'

export async function insertRequest(requestBody) {
    const request = {
        method: 'POST',
        headers: { 'Content-type' : 'text/plain' },
        body: requestBody
    }
    try {
        const response = await fetch(BASE_URL + '/POST', request)
        const json = await response.json()
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