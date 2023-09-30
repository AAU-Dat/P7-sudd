job("Build and push Docker") {
    startOn {
        gitPush {
            anyBranchMatching {
                +"main"
            }
        }
    }

    gradlew("amazoncorretto:17-alpine", "build")
    host("Build artifacts and a Docker image") {
        dockerBuildPush {
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