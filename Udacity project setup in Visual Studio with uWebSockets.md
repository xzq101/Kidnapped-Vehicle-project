
    <div id="sb-site" style="min-height: 722px;">
                <header id="header" class="scrolled">
            <div id="logo">
                ### [codza](https://www.codza.com/)
            </div>
            <div id="navbar">
                                                                

                                    - 
                    [
                                            Home
                    ](https://www.codza.com/)
                
                    

                                <span class="panel-activation sb-toggle-left navbar-left menu-btn fa fa-bars"></span>
            </div>
        </header>
        
        
                <section id="body" class="">
            					
<div id="breadcrumbs" itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb">
                                            <a href="https://www.codza.com/" itemprop="url"><span itemprop="title">Home</span></a>
                        <i class="fa fa-angle-right"></i>
                                                <span itemprop="title">Udacity project setup in Visual Studio with uWebSockets</span>
                        </div>
		
		<div class="blog-content-item grid pure-g-r">
			<div id="item" class="block pure-u-2-3 h-entry">
			    <div class="list-item h-entry">

                
    <div class="list-blog-header">
        <span class="list-blog-date">
            <time class="dt-published" datetime="2018-01-25T13:34:00-07:00">
                <span>25</span>
                *Jan*
            </time>
        </span>
                    #### [Udacity project setup in Visual Studio with uWebSockets](https://www.codza.com/blog/udacity-uws-in-visualstudio)
        
                <span class="tags">
                        [udacity](https://www.codza.com/tag:udacity)
                        [carnd](https://www.codza.com/tag:carnd)
                        [visualstudio](https://www.codza.com/tag:visualstudio)
                        [win10](https://www.codza.com/tag:win10)
                    </span>
                                                                    ![](./Udacity project setup in Visual Studio with uWebSockets _ codza_files/82353363275ad7efcdad4781bfedbbe6c80fc556-0uda.png)
        
    </div>

    <div class="list-blog-padding">

            <div class="e-content">
            How to set up a [Visual Studio](https://www.visualstudio.com/) project for [Udacity](https://www.udacity.com/) projects using [uWebSockets](https://github.com/uNetworking/uWebSockets).

I'm taking Udacity's [self driving car nanodegree](https://www.udacity.com/course/self-driving-car-engineer-nanodegree--nd013). There are several projects in Term2 and Term3 that use the [uWebSockets](https://github.com/uNetworking/uWebSockets) library:

- [CarND-Extended-Kalman-Filter-Project](https://github.com/udacity/CarND-Extended-Kalman-Filter-Project)
- [CarND-Unscented-Kalman-Filter-Project](https://github.com/udacity/CarND-Unscented-Kalman-Filter-Project)
- [CarND-Catch-Run-Away-Car-UKF](https://github.com/udacity/CarND-Catch-Run-Away-Car-UKF)
- [CarND-Kidnapped-Vehicle-Project](https://github.com/udacity/CarND-Kidnapped-Vehicle-Project)
- [CarND-PID-Control-Project](https://github.com/udacity/CarND-PID-Control-Project)
- [CarND-MPC-Project](https://github.com/udacity/CarND-MPC-Project)
- [CarND-Path-Planning-Project](https://github.com/udacity/CarND-Path-Planning-Project)

Visual Studio has [cmake](https://blogs.msdn.microsoft.com/vcblog/2016/10/05/cmake-support-in-visual-studio/) support, but I found working with the MS stack easier. It will take a couple minutes to set up a **Visual Studio** project, but it's worth it.
##### Requirements

- Visual Studio 2017 or Visual Studio 2015 Update 3

##### Install uWebSockets with vcpkg
[Vcpkg](https://github.com/Microsoft/vcpkg) is a tool to install C and C++ libraries. Visual Studio will find the installed libraries automatically, and uWebSockets is available through vcpkg.
Installing **vcpkg** is easy, the instructions from the [vcpkg github](https://github.com/Microsoft/vcpkg) project:

1. Clone the [vcpkg github repo](https://github.com/Microsoft/vcpkg) (in C:\src, but you can use whatever folder you want)
    <pre>`C:\src&gt; git <span class="hljs-keyword">clone</span> https:<span class="hljs-comment">//github.com/Microsoft/vcpkg.git</span>`</pre>
1. Open a command prompt **as administrator** to hook up the integration (only need to do this once)
    <pre>`C:\src\vcpkg&gt; .\vcpkg integrate <span class="hljs-keyword">install</span>`</pre>
1. Install uWebSockets (I chose the x64 version only)
    <pre>`C:\src\vcpkg&gt; .\vcpkg <span class="hljs-keyword">install</span> uwebsockets:x64-windows`</pre>

##### Create VS project
Create an empty Visual C++ project in Visual Studio (**File → New → Project...**). I left **Create new Git repository** unchecked because the Udacity projects have their own git repos on github.
![new empty project](./Udacity project setup in Visual Studio with uWebSockets _ codza_files/new_empty_project.png)
Close Visual Studio to make sure the project is saved.
Fork the Udacity project on [github](ttps://github.com/) (this is optional, but it's easiest to submit the finished project as a github repo).
For simplicity I'll just use the original [CarND-Path-Planning-Project](https://github.com/udacity/CarND-Path-Planning-Project) (without forking):
    C:\Projects\udacity\carnd3&gt; git <span class="hljs-keyword">clone</span> https:<span class="hljs-comment">//github.com/udacity/CarND-Path-Planning-Project.git</span>
Move the **Visual Studio** project and solution files into the cloned project folder:
![copy project files](./Udacity project setup in Visual Studio with uWebSockets _ codza_files/repo_files.png)
(At this point you can delete the now empty folder where the Visual Studio project was created.)
The project folder will look something like this:
![project with vs files](./Udacity project setup in Visual Studio with uWebSockets _ codza_files/project_files.png)
Open the Visual Studio solution by double clicking the **.sln** file. We'll add the **src** files to the solution next.
In the **Solution Explorer** right click **Source Files** and select **Add → Existing Item...** (or use the Shift+Alt+A shortcut):
![solution explorer](./Udacity project setup in Visual Studio with uWebSockets _ codza_files/solution_explorer.png)
Add all source files from the **src** folder (don't add Eigen, we'll include that later):
![src files](./Udacity project setup in Visual Studio with uWebSockets _ codza_files/src_files.png)
##### Fix uWebSockets version issues
Unfortunately [Udacity](https://www.udacity.com/) used an older version of **uWebSockets**. We need to change a couple things in **main.cpp** so it works with the latest version, while making sure that the code also compiles with the older version when the project is submitted for review. We'll do this with [preprocessor directives](http://www.cplusplus.com/doc/tutorial/preprocessor/).
First make sure that the **x64** platform is selected for the solution (if you don't have this setting on the toolbar, you can also change it in **Build → Configuration Manager → Active Solution Platform**):
![x64 platform](./Udacity project setup in Visual Studio with uWebSockets _ codza_files/x64.png)
Select the project in the **Solution Explorer** (in this case **path-planning**) and open up its properties (right click → Properties or Alt+Enter). Make sure that **All Configurations** and **x64** are active. Go to **Configuration Properties → C/C++ → Preprocessor**. Add this to **Preprocessor Definitions** (don't delete what's already there, just paste in front or after):
    UWS_VCPKG;_USE_MATH_DEFINES;_CRT_SECURE_NO_WARNINGS;
Should look like this:
![predefines](./Udacity project setup in Visual Studio with uWebSockets _ codza_files/predefines.png)
Note: We added **_USE_MATH_DEFINES** because **M_PI** is not defined by default. **_CRT_SECURE_NO_WARNINGS** disables errors about **sprintf** being unsecure.
If you build the project now, you'll see several errors. This is because in the latest **uWebSockets** version the server parameter (`uWS::WebSocket&lt;uWS::SERVER&gt; ws`) is a pointer. So we'll take the lines where it's used and put them into a preprocessor condition:
    <span class="hljs-meta">#<span class="hljs-meta-keyword">ifdef</span> UWS_VCPKG</span>
      <span class="hljs-comment">// code fixed for latest uWebSockets</span>
    <span class="hljs-meta">#<span class="hljs-meta-keyword">else</span></span>
      <span class="hljs-comment">// leave original code here</span>
    <span class="hljs-meta">#<span class="hljs-meta-keyword">endif</span></span>
For example:
![code before](./Udacity project setup in Visual Studio with uWebSockets _ codza_files/code_before.png)
Will look like this (added * to make ws a pointer and changed . to -&gt;):
![code fixed](./Udacity project setup in Visual Studio with uWebSockets _ codza_files/code_fixed.png)
You can look at the whole fixed [main.cpp](https://www.codza.com/blog/udacity-uws-in-visualstudio/main.cpp.txt) file [here](https://www.codza.com/blog/udacity-uws-in-visualstudio/main.cpp.txt).
##### Eigen
If for some reason the **Eigen** headers are not found automatically, add the folder where it's located in the project's **Properties → Configuration Properties → C/C++ → General → Additional Include Directories** (in this case **src**):
![includes](./Udacity project setup in Visual Studio with uWebSockets _ codza_files/includes.png)
There is a custom view definition for Eigen objects so they look nice in Visual Studio's debugger. To install:

- get [Eigen.natvis](https://github.com/cdcseacave/Visual-Studio-Visualizers/blob/master/Eigen.natvis) from [this github page](https://github.com/cdcseacave/Visual-Studio-Visualizers/blob/master/Eigen.natvis) (right click on **Raw → Save link as...** ).
- copy it to the project's folder
- add as **Existing Item** to **Source Files** in the **Solution Explorer**

![natvis](./Udacity project setup in Visual Studio with uWebSockets _ codza_files/natvis.png)
##### Fix simulator connection
One last thing: we have to tell **uWebSockets** to listen on **127.0.0.1**, otherwise the simulator won't connect.
Modify this line:
        <span class="hljs-keyword">if</span> (h.listen(port)) {
to this:
        <span class="hljs-keyword">if</span> (h.listen(<span class="hljs-string">"127.0.0.1"</span>, port)) {
And that's it! You can now code in Visual Studio and the reviewer will be able to compile your submission using **cmake**. As long as you add new source files to the **src** folder, **cmake** will pick them up.
        </div>

        
            
    
        &#10;                            [<i class="fa fa-chevron-left"></i> Next Post](https://www.codza.com/blog/mysql-server-not-configured-error)
            &#10;                            [Previous Post <i class="fa fa-chevron-right"></i>](https://www.codza.com/blog/lamp-in-wsl-win10)
                    
    
    </div>
</div>
			</div>
			<div id="sidebar" class="block size-1-3 pure-u-1-3">
				
<div class="sidebar-content">
    <h4>Search</h4>
    <div class="search-wrapper">
    <form name="search" data-simplesearch-form="">
        <input name="searchfield" class="search-input" type="text" min="3" required="" placeholder="Search …" value="" data-search-invalid="Please add at least 3 characters" data-search-separator=":" data-search-input="/search/query">
            </form>
</div>
</div>
    #### Related Posts
    
                                - 
                                        <span class="score">50</span>
                                    [Zsh in Windows 10 WSL](https://www.codza.com/blog/zsh-in-wsl-win10 "Zsh in Windows 10 WSL")
                
                                        - 
                                        <span class="score">50</span>
                                    [LAMP stack in Windows 10 WSL](https://www.codza.com/blog/lamp-in-wsl-win10 "LAMP stack in Windows 10 WSL")
                
                                        - 
                                        <span class="score">50</span>
                                    [ERROR: Package mysql-server-5.7 is not configured yet.](https://www.codza.com/blog/mysql-server-not-configured-error "ERROR: Package mysql-server-5.7 is not configured yet.")
                
                                        - 
                                        <span class="score">50</span>
                                    [Fix SYSTEM_THREAD_EXCEPTION_NOT_HANDLED Blue Screen of Death on Win10](https://www.codza.com/blog/system-thread-exception-not-handled-bsod-fix "Fix SYSTEM_THREAD_EXCEPTION_NOT_HANDLED Blue Screen of Death on Win10")
                
            <div class="sidebar-content">
	#### Random Article
	[<i class="fa fa-retweet"></i> I'm Feeling Lucky!](https://www.codza.com/random)
</div>


<div class="sidebar-content">
    <h4>Popular Tags</h4>
    
<span class="tags">
                    <a class="" href="https://www.codza.com/tag:win10">win10</a>
                    <a class="" href="https://www.codza.com/tag:wsl">wsl</a>
                    <a class="" href="https://www.codza.com/tag:ubuntu">ubuntu</a>
                    <a class="" href="https://www.codza.com/tag:udacity">udacity</a>
                    <a class="" href="https://www.codza.com/tag:carnd">carnd</a>
                    <a class="" href="https://www.codza.com/tag:visualstudio">visualstudio</a>
                    <a class="" href="https://www.codza.com/tag:mysql">mysql</a>
                    <a class="" href="https://www.codza.com/tag:debian">debian</a>
                    <a class="" href="https://www.codza.com/tag:bsod">bsod</a>
                    <a class="" href="https://www.codza.com/tag:zsh">zsh</a>
                    <a class="" href="https://www.codza.com/tag:lamp">lamp</a>
    </span>
</div>
<div class="sidebar-content">
    <h4>Archives</h4>
	<ul class="archives">

    <li>
    	<a href="https://www.codza.com/archives_month:feb_2018">
                <span class="label">1</span>
                <span class="archive_date">February 2018</span>
        </a>
    </li>
    <li>
    	<a href="https://www.codza.com/archives_month:jan_2018">
                <span class="label">4</span>
                <span class="archive_date">January 2018</span>
        </a>
    </li>
</ul>
</div>
<div class="sidebar-content syndicate">
    <h4>Syndicate</h4>
    <a class="button" href="https://www.codza.com/blog.atom"><i class="fa fa-rss-square"></i> Atom 1.0</a>
    <a class="button" href="https://www.codza.com/blog.rss"><i class="fa fa-rss-square"></i> RSS</a>
</div>
			</div>
		</div>

	        </section>
        
                <footer id="footer">
            <div class="totop">
                <span>[<i class="fa fa-arrow-up"></i>](https://www.codza.com/blog/udacity-uws-in-visualstudio#)</span>
            </div>
            [Grav](http://getgrav.org/) was <i class="fa fa-code"></i> with <i class="fa fa-heart"></i> by [RocketTheme](http://www.rockettheme.com/).
        </footer>
            </div>
            <div class="sb-slidebar sb-left sb-width-thin">
            <div id="panel">
            

                                    - 
                    [
                                            Home
                    ](https://www.codza.com/)
                
                    

            </div>
        </div>
                

        
    

