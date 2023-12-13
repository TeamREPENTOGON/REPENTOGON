/////// REPENTOGON ADDITIONS //////////
function urlExists(url, callback){
    $.ajax({
      type: 'HEAD',
      url: url,
      success: function(){
        callback(true);
      },
      error: function() {
        return false;
      }
    });
  }

function addOriginalDocLink(){
    var splitURL = $(location).attr("href").split("/");
    var className = splitURL[splitURL.length-1].split(".")[0];
    var url = "https://wofsauge.github.io/IsaacDocs/rep/"+className+".html";
    urlExists(url, function(exists){
        if(className != "index"){
            var linkObj = $("<h3>This class is an extension of the original \"<a href=\""+url+"\">"+className+"</a>\" class!</h3>");
            linkObj.insertAfter($(".md-content__inner").find("h1").first());

            var ToCElement = $('<p style="margin: 0.5em;line-height: 0.6;" class="md-typeset md-button"> <a style="display: grid; text-align: center;" class="md-button" href="'+url+'">Go to original docs</a></p>');
        
            $(".md-sidebar.md-sidebar--secondary").first().prepend(ToCElement);
        }
      });
}

//////////////////////////////////////
$(document).ready(function() {
    var check = "checked";
    if (typeof(Storage) !== "undefined" && localStorage.getItem("highlightResults") == 1) {
        check = ""
    }
    $("div.md-search-result").prepend('<div class="clearSearchMarks"><input type="checkbox" onclick="unmarkStuff()" id="searchMarks" name="scales" ' + check + '><label for="searchMarks">Highlight search results</label></div>');
});

$(document).ready(function() {
    if (window.location.hash.replace("#", "") != "") {
        jumpToElement($(window.location.hash));
    }
});

function unmarkStuff() {
    if (typeof(Storage) !== "undefined") {
        var highlightResultsState = localStorage.getItem("highlightResults");
        if (highlightResultsState == null || highlightResultsState == 0) {
            highlightResultsState = 1;
            $content.unmark();
            $("body").addClass("hideMarks");
            $(".clearSearchMarks").prop('checked', false);
        } else {
            highlightResultsState = 0;
            $("body").removeClass("hideMarks");
            $(".clearSearchMarks").prop('checked', true);
        }

        $("li.md-search-result__item").find('a').each(function(e) {
            var jumpTargetValue = $(this).attr('href').split("#");
            var jumpTarget = "";
            if (jumpTargetValue.length > 1) {
                jumpTarget = "#" + jumpTargetValue[1];
            }
            var searchText = "?h=" + $("input[aria-label=\"Search\"]").val();
            var link = $(this).attr('href').split("?")[0].split("#")[0];
            $(this).attr('href', link + searchText + jumpTarget);
        });

        localStorage.setItem("highlightResults", highlightResultsState);
    }
}

function reevaluateLastVisit() {
    if (typeof(Storage) !== "undefined") {
        $(".md-nav label:contains('Last visited')").parent().find("a").each(function(index) {
            var lastVisitEntry = getRecentList()[index];
            if (lastVisitEntry !== undefined) {
                $(this).attr("href", lastVisitEntry);
                lastVisitEntry = lastVisitEntry.replace("IsaacDocs/", "").replace("rep/", "").replace("abp/", "").replace("docs/", "");
                var linkName = lastVisitEntry.substring(1, lastVisitEntry.length).replace(".html", "");
                $(this).text(linkName);
            } else {
                $(this).parent().hide();
            }
        });

    } else {
        $(".md-nav label:contains('Last visited')").parent().hide();
    }
}

function getRecentList() {
    var recentList = localStorage.getItem("recentlyVisited");
    if (recentList == null) {
        recentList = [];
    } else {
        recentList = recentList.split(',');
    }
    return recentList;
}

function buildContentMap() {
    if (!$("h1").first().text().includes("Class ")) {
        //only build map on class-related pages
        return;
    }
    var mapObj = $("<div class=\"contentMap\"><h2 class=\"overviewHeader\">Content Overview</h2><table class=\"contentTable\" id=\"contentOverviewTable\"><thead><tr><th>Return value</th><th>Function</th></tr></thead><tbody></tbody></table><hr/></div>");
    if ($("#class-diagram").length == 0) {
        mapObj.insertBefore($(".md-content__inner").find("h2").first());
    } else {
        // insert content map after mermaid diagram
        mapObj.insertAfter($(".mermaidDiagram").first());
    }

    var tableContent = "";
    $("h4").each(function(index) {
        //remove anchor links and mark objects from variable description headers... we dont need them and they suck
        var cloneH4 = $(this).clone();
        cloneH4.find("a.headerlink").remove();
        cloneH4.find("mark").contents().unwrap();
        var funcParts = cloneH4.html().split(" (");
        var funcFront = funcParts[0].split(" ");
        var funcName = funcFront.pop();
        var parentH3Node = $(this).prev();
        while (parentH3Node.prop("tagName") != "H3") {
            parentH3Node = parentH3Node.prev();
        }
        var funcLink = parentH3Node.find("a").last().attr("href");
        funcName = "<a href=\"" + funcLink + "\">" + funcName + "</a>";
        var ariaLabel = $(this).attr("aria-label");
        if (funcParts.length > 1) {
            tableContent = tableContent + "<tr aria-label=\"" + ariaLabel + "\"><td>" + funcFront.join(" ") + "</td><td aria-label=\"" + ariaLabel + "\" class=\"copyable\">" + funcName + " (" + funcParts[1] + "</td></tr>";
        } else {
            tableContent = tableContent + "<tr aria-label=\"" + ariaLabel + "\"><td>" + funcFront.join(" ") + "</td><td aria-label=\"" + ariaLabel + "\" class=\"copyable\">" + funcName + "</td></tr>";
        }
    });

    $('#contentOverviewTable > tbody').append(tableContent);
}

function trimSearchResults(){
    // Trim search results exceeding a given size
    $(".md-search-result__article mark").each(function (index) {
        var pElements = $(this).parent().contents();
        pElements.each(function (index2) {
          if (this.nodeName == "MARK") {
            if (index2 - 1 >= 0 && pElements[index2 - 1].nodeType == 3) {
              // raw text element preceeding the <mark> object
              var oldText = pElements[index2 - 1].textContent;
              if (oldText.length > 100) {
                pElements[index2 - 1].textContent = "..." + oldText.substring(oldText.length - 90, oldText.length);
              }
            }
            // raw text element between two <mark> objects
            if (index2 + 1 < pElements.length && pElements[index2 + 1].nodeType == 3 && index2 + 2 < pElements.length && pElements[index2 + 2].nodeName == "MARK") {
              var oldText = pElements[index2 + 1].textContent;
              if (oldText.length > 100) {
                pElements[index2 + 1].textContent = oldText.substring(0, 45) + " ... " + oldText.substring(oldText.length - 45, oldText.length);
              }
            }
            if (index2 + 1 < pElements.length && pElements[index2 + 1].nodeType == 3) {
              // raw text element trailing the <mark> object
              var oldText = pElements[index2 + 1].textContent;
              if (oldText.length > 100) {
                pElements[index2 + 1].textContent = oldText.substring(0, 90) + "...";
              }
            }
          }
        });
      });
}

function modifyCallbackPageLayout() {
    if (!window.location.pathname.includes("ModCallbacks")) {
        return;
    }

    var tableContent = "";
    $("article.md-content__inner .md-typeset__table").each(function(index) {
        var headerLink = $(this).find("td:eq(1)").text().toLowerCase();
        tableContent = tableContent + "<tr>" +
            "<td class=\"copyable\" style=\"text-align: right;\"><a href=\"#" + headerLink + "\">" + $(this).find("td:eq(1)").text() + "</a></td>" +
            "<td>" + $(this).find("td:eq(2)").html() + "</td>" +
            "<td>" + $(this).find("td:eq(3)").html() + "</td>" +
            "<td>" + $(this).find("td:eq(4)").html() + "</td></tr>";
    });
    var mapObj = $("<div class=\"contentMap\"><h2 class=\"overviewHeader\">Content Overview</h2><table class=\"contentTable\" id=\"contentOverviewTable\">" +
        "<thead><tr><th>Name</th><th>Function Args</th><th>Optional Args</th><th>Return Type</th></tr></thead><tbody></tbody></table><hr/></div>");
    mapObj.insertAfter($(".md-content__inner").find("p").first());

    $('#contentOverviewTable > tbody').append(tableContent);
}

function addBitsetCalculator() {
    $(".bitsetCalculator").each(function(header) {
        $("<div id=\"bitsetCalculator\"><label for=\"bitsetCalcInput\" style=\"float:left\">Number:</label><input type=\"number\" min=\"0\" class=\"md-search__input\" id=\"bitsetCalcInput\" name=\"bitsetCalcInput\"><span id=\"bitsetCalcSplits\"></span><div id=\"bitsetCalcResult\"></div></div>").insertAfter($(this));
        $("#bitsetCalcInput").on('input', function() {
            $("#bitsetCalcSplits").empty();
            $("#bitsetCalcResult").empty();
            $(".md-typeset__table").find("tr").removeClass("tableHighlight");
            if ($("#bitsetCalcInput").val() == "") {
                return;
            }

            if (parseInt($("#bitsetCalcInput").val()) == "0") {
                var isMatched = false;
                $(".md-typeset__table").find("tr").each(function(index) {
                    var val = $(this).find("td:eq(1)");
                    if (val.length > 0) {
                        val = val.text();
                        val = val.replace("BitSet128(0,0)", "0");
                        if ("0" === val) {
                            isMatched = true;
                            $(this).addClass("tableHighlight");
                        }
                    }
                });
                var classVar = isMatched ? "class=\"highVal\"" : "";
                $("#bitsetCalcSplits").append($("<span " + classVar + ">" + $("#bitsetCalcInput").val() + "</span>"));
                $("#bitsetCalcResult").append($("<span>Found " + (isMatched ? 1 : 0) + " matching enums.</span>"));

            } else {
                var binary = BigInt($("#bitsetCalcInput").val()).toString(2);
                var len = binary.length - 1;
                var completeCounter = 0;
                for (let i = len; i >= 0; i--) {
                    var bit = parseInt(binary.substring(len - i, len - i + 1));
                    var bitToFull = BigInt(bit) << BigInt(i);
                    var isMatched = false;
                    $(".md-typeset__table").find("tr").each(function(index) {
                        var val = $(this).find("td:eq(1)");
                        if (val.length > 0 && bit == 1) {
                            val = val.text();
                            val = val.replace("BitSet128(0,0)", "0");
                            if (val.indexOf("TEARFLAG") >= 0) {
                                val = val.replace("TEARFLAG(", "").replace(")", "");
                                val = "1<<" + val;
                            }
                            if (val.indexOf("<<") >= 0) {
                                val = BigInt(1) << BigInt(parseInt(val.split("<<")[1]));
                            }
                            if (bitToFull == val) {
                                $(this).addClass("tableHighlight");
                                isMatched = true;
                                completeCounter = completeCounter + 1;
                                return false;
                            }
                        }
                    });
                    var classVar = bit == 1 && isMatched ? "class=\"highVal\"" : "";
                    $("#bitsetCalcSplits").append($("<span " + classVar + ">" + bit + "</span>"));
                    if (i % 4 == 0 && i != 0) {
                        $("#bitsetCalcSplits").append($("<span>|</span>"));
                    }
                }
                $("#bitsetCalcResult").append($("<span>Found " + completeCounter + " matching enums.</span>"));
            }

        });
        $("<span>Highlights all bit-flag enums that construct the given integer value.</span><br>").insertAfter($(this));
    });
}

document$.subscribe(function() {
    if (typeof(Storage) !== "undefined") {
        // handle recently visited
        var recentList = getRecentList();

        var pathname = window.location.pathname;
        if (pathname !== "/" && !pathname.includes("index.html")) {
            const index = recentList.indexOf(pathname);
            if (index > -1) {
                recentList.splice(index, 1);
            }
            recentList.unshift(pathname);
            recentList.splice(10, 1);
            localStorage.setItem("recentlyVisited", recentList.toString());
        }
    }
    // handle badge line object
    $(".badge").each(function(table) {
        if ($(this).parent().prop("tagName") == "P") {
            $(this).parent().addClass("badgeLine");
        }
    });

    modifyCallbackPageLayout();
    addBitsetCalculator();
    buildContentMap();
    addOriginalDocLink();

    // reduce audio volume to 25%
    $("audio").prop("volume", 0.25);

    $(".overviewHeader").click(function() {
        $(this).toggleClass("collapsed");
        $(".contentTable").toggle();
    });

    // handle frequently used Entry
    $(".md-nav__link:contains('Frequently used')").addClass("frequentlyUsed");

    $(".md-nav label:contains('Last visited')").siblings().click(function() {
        reevaluateLastVisit();
    });

    //moves scroll position on href clicking a bit further up
    $('a[href^="#"]').on('click', function(e) {
        var href = $(this).attr('href');
        jumpToElement(href);
    });

    // Make tables sortable
    document.querySelectorAll("article table").forEach(function(table) {
        new Tablesort(table)
    })

    // handle Copy Buttons
    $(".copyable").each(function(e) {
        if ($(this).prop("tagName") == "TD") {
            $(this).attr('id', $(this).text())
            $(this).append('<a class="headerlink" href="#' + $(this).text() + '" title="Permanent link">⚓︎</a>');
        }
        $(this).append('<button class="md-clipboard copyButton md-icon"><span>Copy to clipboard</span></button>');
    })


    $(".copyButton").click(function() {
        var parent = $(this).parent();
        $(this).find("span").first().text("");

        var pathname = window.location.pathname;
        pathname = pathname.replace(".html", "").substring(1, pathname.length - 1);
        var splitted = pathname.split("/");
        pathname = splitted[splitted.length - 1];
        var funcNameLine = "";
        parent.each(function(index) {
            funcNameLine = $(this).text();
        });
        var functionString = funcNameLine.replace("⚓︎", "");
        if (funcNameLine.includes("(")) {
            functionString = funcNameLine.replace(" ( ", "(");
            var funcPart1 = functionString.split("(")[0].split(" ");
            var p1 = funcPart1[funcPart1.length - 1];
            var funcPart2 = "";
            var funcVars = functionString.replace(functionString.split("(")[0], "")
            $.each(funcVars.split(", "), function(index, value) {
                if (index > 0) {
                    funcPart2 += ", ";
                }
                if (value.split(" ").length > 1) {
                    funcPart2 += value.split(" ")[1].replace(" )", "").replace(")", "");
                }
            });
            functionString = p1 + "(" + funcPart2 + ")";
        } else {
            functionString = functionString.split(" ")[functionString.split(" ").length - 1]
        }

        var connector = ".";
        var isGlobalClass = $("h1").first().text().includes("Global ");
        if (funcNameLine.includes("(") && !isGlobalClass && !pathname.includes("Isaac") && !pathname.includes("Input")) {
            connector = ":";
        }
        if (parent.attr("aria-label") == "Constructors") {
            connector = "";
        }
        if (!window.location.pathname.includes("enums") && !isGlobalClass && !pathname.includes("Isaac") && !pathname.includes("Input")) {
            pathname = "";
        }

        var copyText = pathname + connector + functionString;

        copyText = copyText.replace("Copy to clipboard", "");
        parent.append('<textarea>' + copyText + '</textarea>');
        parent.find("textarea").each(function(index) {
            $(this).select();
            document.execCommand("copy");
            $(this).remove();
        });
        $(this).find("span").first().html("Copied: <br><code>" + copyText+"</code>");
    });

    $(".copyButton").mouseleave(function() {
        $(this).find("span").first().text("Copy to clipboard");
    });

    //Adds search query string to search result links
    // We use an Element observer, to change the search results AFTER they where placed
    var target = document.querySelector('.md-search-result__list')
    var observer = new MutationObserver(function(mutations) {
        $("li.md-search-result__item").each(function(e) {
            var firstATag = $(this).find('a').first();
            colorizeSearchResults(firstATag);
        });
        $("article.md-search-result__article").each(function(e) {
            if ($(this).attr("data-md-score") < 0) {
                $(this).parent().parent().hide();
            }
        });

        trimSearchResults();
    });
    var config = { attributes: true, childList: true, characterData: true };
    observer.observe(target, config);

    //Add Edit button to each header
    var editButton = $(".md-content__button.md-icon")
    $("h2>.headerlink,h3>.headerlink").each(function(e) {
        var editClone = editButton.clone();
        editClone.addClass("inlineEditButton");
        editClone.addClass("headerlink");
        editClone.appendTo($(this).parent());
    })

    mark();

    if (typeof(Storage) !== "undefined") {
        if (localStorage.getItem("highlightResults") == 1) {
            $(".clearSearchMarks").prop('checked', false);
            $content.unmark();
            $("body").addClass("hideMarks");
        }
    }
});

// mark.js functionality
var $results;
var $content = $("article");
var mark = function() {
    // Read the url
    var sPageURL = window.location.search;
    if (sPageURL.indexOf("?") !== -1) {

        // Generate an array with all referrer parameters
        var qs = sPageURL.substr(sPageURL.indexOf('?') + 1);
        var qsa = qs.split('&');

        // Get search keywords
        var keyword = "";
        for (var i = 0; i < qsa.length; i++) {
            var currentParam = qsa[i].split('=');
            if (currentParam.length !== 2) {
                continue;
            }
            if (currentParam[0] == "h") {
                keyword = decodeURIComponent(currentParam[1].replace(/\+/g, "%20"));
            }
        }

        if (keyword != "") {
            // Mark the keyword inside the context
            $content.unmark({
                done: function() {
                    $content.mark(keyword, {
                        separateWordSearch: true,
                        done: function() {
                            $results = $content.find("mark");
                            jumpToFirst();
                        }
                    });
                }
            });
        }
    }
};

function jumpToFirst() {
    if ($results.length) {
        var position,
            $current = $results.eq(0);
        if ($current.length) {
            position = $current.offset().top - 75;
            window.scrollTo(0, position);
        }
    }
    if (window.location.hash.replace("#", "") != "") {
        jumpToElement($(window.location.hash));
    }
}


function jumpToElement(element) {
    $('html, body').animate({
        scrollTop: $(element).offset().top - 75
    }, 5);
}

function colorizeSearchResults(element) {
    var text = element.text();
    if (text.includes("Class ")) {
        element.addClass("searchClass");
    } else if (text.includes("Tutorial")) {
        element.addClass("searchTutorial");
    } else if (text.includes("Enum ")) {
        element.addClass("searchEnum");
    } else if (text.includes(".xml")) {
        element.addClass("searchXML");
    } else if (text.includes("File ")) {
        element.addClass("searchFile");
    } else if (text.includes("FAQ")) {
        element.addClass("searchFAQ");
    }
}



////////////////////////////////////////////////
  var questionHistory = [];
  var currentQuestion;

  function createAnswers() {
    $("#faq_buttons").empty();
    if ("answers" in currentQuestion) {
      for (const answer of currentQuestion.answers) {
        var answerButton = $('<button class="faqButton md-button md-button--primary" onclick="setQuestion(\'' + answer.link + "')\">" + answer.text + "</button>");
        $("#faq_buttons").append(answerButton);
      }
    }
  }
  function previousQuestion() {
    var prevQuestion = "START";
    if (questionHistory.length > 1) {
      prevQuestion = questionHistory[questionHistory.length-2];
    }
    setQuestion(prevQuestion, true);
    questionHistory.pop();
  }

  function setQuestion(questionID, isUndo) {
    if (typeof INTERACTIVE_questions === "undefined") {
      return;
    }
    $("#interactiveFAQ").fadeOut(250, function () {
      // Add to history
      if (!isUndo) {
        questionHistory.push(questionID.toString());
      }
      currentQuestion = INTERACTIVE_questions[questionID.toString()];

      $("#faq_text").html(currentQuestion.text);

      if ("image" in currentQuestion) {
        $("#faq_image").attr("src", currentQuestion.image);
      } else {
        $("#faq_image").attr("src", "");
      }

      createAnswers();

      $("#interactiveFAQ").fadeIn(250);
    });
  }

  document$.subscribe(function () {
    $(".interactiveFAQ").each(function (header) {
      $('<div id="interactiveFAQ"><img id="faq_image" width="400" align="right" alt="" src=""><h4>Question</h4><div id="faq_text"></div><div id="faq_buttons"></div></div>').insertAfter($(this));
      setQuestion("START");
    });
  });




