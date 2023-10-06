job("Build and push Docker") {
    startOn {
        gitPush {
            anyBranchMatching {
                +"main"
            }
        }
    }

    failOn {
        timeOut {
            runningTimeOutInMinutes = 200
        }
    }

    host("Build and push a Docker image") {
        dockerBuildPush {
            file = "Dockerfile.base"
            labels["vendor"] = "gg-season"
            args["number_of_threads"] = "1"

            val spaceRepo = "gg-summer.registry.jetbrains.space/p/p7/markov/jajapy"
            tags {
                +"$spaceRepo:1.0.${"$"}JB_SPACE_EXECUTION_NUMBER"
                +"$spaceRepo:latest"
            }
        }
    }
}