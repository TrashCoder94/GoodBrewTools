# Commit Checker

## Instructions

1. Go to `ToolsForTheToolsRepo\CommitChecker` and run `CommitChecker.exe`
   
2. You should see a window like this
   -  ![commitchecker-mainwindow](https://github.com/TrashCoder94/GoodBrewTools/assets/94650958/2d71b434-7cb6-48d1-be8a-a80aa81f04b0)

3. Fill out the title and description fields to describe what changes you have made:
   - ![commitchecker-mainwindow-titleanddescription](https://github.com/TrashCoder94/GoodBrewTools/assets/94650958/a961d634-aaa3-4fca-b960-8b240b4f8bed)

4. Click on the `Targets` button and select which platform(s)/configuration(s) you want to build with your local changes:
   - ![commitchecker-mainwindow-targetsbutton](https://github.com/TrashCoder94/GoodBrewTools/assets/94650958/b7441b12-0882-4712-b187-07825a627856)
   - ![commitchecker-targetswindow](https://github.com/TrashCoder94/GoodBrewTools/assets/94650958/eb32cc75-450b-4735-9b7f-808f6c904072)
   - By default, Linux Debug will be checked but feel free to select/deselect other options and then press `Save And Close` to save your selection.

5. Next, click on the `Compile` button:
   - ![commitchecker-mainwindow-compilebutton](https://github.com/TrashCoder94/GoodBrewTools/assets/94650958/61252e03-e76b-437b-ad1e-8745981ea1ee)
   - ![commitchecker-mainwindow-compiling](https://github.com/TrashCoder94/GoodBrewTools/assets/94650958/08ec1bbf-03b8-4862-b3e4-55a474d8c415)
   - You should see the compile button update to `Compiling...` with a spinning icon as shown above to show that it's working.
   - ![commitchecker-mainwindow-compilesuccessful](https://github.com/TrashCoder94/GoodBrewTools/assets/94650958/002a383c-ca3d-463c-bbd1-8cbb8287db33)
   - If the compile was successful, you should see a green tick next to the `Compile` button above and the `Commit` button should become enabled.
   - ![commitchecker-mainwindow-compilefailed](https://github.com/TrashCoder94/GoodBrewTools/assets/94650958/c88c0e0a-6b9e-4390-8637-bce5e3ad2f7e)
   - ![commitchecker-warningsorerrorswindow](https://github.com/TrashCoder94/GoodBrewTools/assets/94650958/499aef11-f5d3-4703-a9d3-26fc42af837b)
   - If the compile failed, you should see a red cross next to the `Compile` button and the `Warnings Or Errors Window` should pop up showing what the warnings or errors were. You can copy the message to the clipboard. You will have to fix the warnings or errors before being able to `Commit` anything.

6. Next, click on the `Commit` button:
   - ![commitchecker-mainwindow-commitbutton](https://github.com/TrashCoder94/GoodBrewTools/assets/94650958/3ab1d9bd-a14d-4ed1-aafe-e3a8e7361e93)
   - ![commitchecker-mainwindow-committing](https://github.com/TrashCoder94/GoodBrewTools/assets/94650958/b9a678d9-39ec-4ba5-a0dd-0889630de6b3)
   - ![commitchecker-mainwindow-commitsuccessful](https://github.com/TrashCoder94/GoodBrewTools/assets/94650958/cce75935-3cab-4893-a86a-0d0ba26ca398)
   - Once the commit has been pushed, a green tick should show up next to the `Commit` button and the application should automatically close itself down.
