{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 8,
			"minor" : 5,
			"revision" : 8,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"classnamespace" : "box",
		"rect" : [ 561.0, 162.0, 801.0, 754.0 ],
		"bglocked" : 0,
		"openinpresentation" : 1,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 1,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 1,
		"objectsnaponopen" : 1,
		"statusbarvisible" : 2,
		"toolbarvisible" : 1,
		"lefttoolbarpinned" : 0,
		"toptoolbarpinned" : 0,
		"righttoolbarpinned" : 0,
		"bottomtoolbarpinned" : 0,
		"toolbars_unpinned_last_save" : 0,
		"tallnewobj" : 0,
		"boxanimatetime" : 200,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"description" : "",
		"digest" : "",
		"tags" : "",
		"style" : "",
		"subpatcher_template" : "",
		"assistshowspatchername" : 0,
		"boxes" : [ 			{
				"box" : 				{
					"id" : "obj-16",
					"maxclass" : "live.line",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 390.942927002906799, 171.0, 5.0, 100.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 0.0, 0.0, 25.0, 160.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontsize" : 10.0,
					"id" : "obj-39",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 260.0, 97.822609522342674, 55.0, 18.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 143.999304286217694, 0.957287886142723, 41.0, 18.0 ],
					"text" : "Sensor"
				}

			}
, 			{
				"box" : 				{
					"fontsize" : 10.0,
					"id" : "obj-38",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 194.353125423192978, 413.585807244777698, 55.0, 18.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 143.999304286217694, 41.8637023577094, 69.259258627891541, 18.0 ],
					"text" : "Threshold"
				}

			}
, 			{
				"box" : 				{
					"fontsize" : 10.0,
					"id" : "obj-37",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 332.0, 537.585807244777698, 48.0, 18.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 143.999304286217694, 102.8637023577094, 42.0, 18.0 ],
					"text" : "Toggle"
				}

			}
, 			{
				"box" : 				{
					"fontsize" : 10.0,
					"id" : "obj-34",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 593.5, 544.0, 36.0, 18.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 26.999304286217694, 104.8637023577094, 42.0, 18.0 ],
					"text" : "Bang"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-11",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 222.795582681894302, 591.585807244777698, 150.0, 20.0 ],
					"text" : "toggle on edge-detection"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-10",
					"linecount" : 2,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 478.0, 595.585807244777698, 150.0, 33.0 ],
					"text" : "bang on edge-detection (up and down)"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.2, 0.2, 0.2, 0.0 ],
					"id" : "obj-32",
					"maxclass" : "led",
					"numinlets" : 1,
					"numoutlets" : 1,
					"oncolor" : [ 0.270588235294118, 0.76078431372549, 0.172549019607843, 1.0 ],
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 560.0, 533.585807244777698, 24.0, 24.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 10.050220928788121, 129.085807244777698, 18.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.2, 0.2, 0.2, 0.0 ],
					"id" : "obj-33",
					"maxclass" : "led",
					"numinlets" : 1,
					"numoutlets" : 1,
					"oncolor" : [ 0.270588235294118, 0.76078431372549, 0.172549019607843, 1.0 ],
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 560.0, 471.585807244777641, 24.0, 24.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 10.050220928788121, 110.835807244777698, 18.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-27",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 363.0, 399.0, 24.0, 24.0 ]
				}

			}
, 			{
				"box" : 				{
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"enablehscroll" : 0,
					"enablevscroll" : 0,
					"id" : "obj-26",
					"lockeddragscroll" : 0,
					"lockedsize" : 0,
					"maxclass" : "bpatcher",
					"name" : "M4L.IO.Remote.maxpat",
					"numinlets" : 3,
					"numoutlets" : 2,
					"offset" : [ 0.0, 0.0 ],
					"outlettype" : [ "int", "" ],
					"patching_rect" : [ 477.795582681894302, 506.134584954977072, 101.204417318105698, 18.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 26.999304286217694, 138.585807244777698, 95.0, 15.0 ],
					"viewvisibility" : 1
				}

			}
, 			{
				"box" : 				{
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"enablehscroll" : 0,
					"enablevscroll" : 0,
					"id" : "obj-22",
					"lockeddragscroll" : 0,
					"lockedsize" : 0,
					"maxclass" : "bpatcher",
					"name" : "M4L.IO.Remote.maxpat",
					"numinlets" : 3,
					"numoutlets" : 2,
					"offset" : [ 0.0, 0.0 ],
					"outlettype" : [ "int", "" ],
					"patching_rect" : [ 477.795582681894302, 568.134584954977072, 101.204417318105698, 18.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 26.999304286217694, 120.335807244777698, 95.0, 15.0 ],
					"viewvisibility" : 1
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-21",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 378.692927002906799, 493.099099099636078, 29.5, 22.0 ],
					"text" : "!= 1"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.2, 0.2, 0.2, 0.0 ],
					"id" : "obj-19",
					"maxclass" : "led",
					"numinlets" : 1,
					"numoutlets" : 1,
					"oncolor" : [ 0.270588235294118, 0.76078431372549, 0.172549019607843, 1.0 ],
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 300.0, 533.585807244777698, 24.0, 24.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 127.050220928788121, 137.085807244777698, 18.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"enablehscroll" : 0,
					"enablevscroll" : 0,
					"id" : "obj-20",
					"lockeddragscroll" : 0,
					"lockedsize" : 0,
					"maxclass" : "bpatcher",
					"name" : "M4L.IO.Remote.maxpat",
					"numinlets" : 3,
					"numoutlets" : 2,
					"offset" : [ 0.0, 0.0 ],
					"outlettype" : [ "int", "" ],
					"patching_rect" : [ 222.795582681894302, 568.134584954977072, 101.204417318105698, 18.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 143.999304286217694, 138.585807244777698, 95.0, 15.0 ],
					"viewvisibility" : 1
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.2, 0.2, 0.2, 0.0 ],
					"id" : "obj-17",
					"maxclass" : "led",
					"numinlets" : 1,
					"numoutlets" : 1,
					"oncolor" : [ 0.270588235294118, 0.76078431372549, 0.172549019607843, 1.0 ],
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 300.0, 471.585807244777641, 24.0, 24.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 127.050220928788121, 118.835807244777698, 18.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"enablehscroll" : 0,
					"enablevscroll" : 0,
					"id" : "obj-13",
					"lockeddragscroll" : 0,
					"lockedsize" : 0,
					"maxclass" : "bpatcher",
					"name" : "M4L.IO.Remote.maxpat",
					"numinlets" : 3,
					"numoutlets" : 2,
					"offset" : [ 0.0, 0.0 ],
					"outlettype" : [ "int", "" ],
					"patching_rect" : [ 222.795582681894302, 506.134584954977072, 101.204417318105698, 18.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 143.999304286217694, 120.335807244777698, 95.0, 15.0 ],
					"viewvisibility" : 1
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-12",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 332.0, 399.0, 24.0, 24.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-5",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 324.0, 433.585807244777698, 24.0, 24.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-3",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "", "int", "int" ],
					"patching_rect" : [ 312.0, 355.0, 48.0, 22.0 ],
					"text" : "change"
				}

			}
, 			{
				"box" : 				{
					"comment" : "",
					"id" : "obj-1",
					"index" : 0,
					"maxclass" : "inlet",
					"numinlets" : 0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 66.692927002906799, 29.697986781597137, 30.0, 30.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-36",
					"justification" : 1,
					"maxclass" : "live.line",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 56.003406524658203, 433.585807244777698, 5.0, 100.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 37.906722068786621, 61.287240960597984, 12.151925939917533, 8.793699443340302 ]
				}

			}
, 			{
				"box" : 				{
					"fontface" : 2,
					"fontname" : "Arial",
					"fontsize" : 10.0,
					"id" : "obj-35",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 138.26606422662735, 125.112103521823883, 122.0, 18.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 89.832744843959745, 77.666655109226696, 33.0, 18.0 ],
					"text" : "invert"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-30",
					"justification" : 1,
					"maxclass" : "live.line",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 71.891615524888039, 433.585807244777698, 5.0, 100.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 119.213915172815291, 82.269805387556545, 9.512902159094779, 8.793699443340302 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-29",
					"justification" : 1,
					"maxclass" : "live.line",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 135.444451525807381, 433.585807244777698, 5.0, 100.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 78.647027417719301, 67.252352626025669, 10.638124242424965, 19.436904840171337 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-25",
					"justification" : 1,
					"maxclass" : "live.line",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 40.115197524428368, 433.585807244777698, 5.0, 100.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 83.453995333909972, 82.269805387556545, 9.866962332129447, 8.793699443340302 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-23",
					"justification" : 1,
					"maxclass" : "live.line",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 119.556242525577545, 433.585807244777698, 5.0, 100.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 78.643587673008426, 63.287240960597984, 50.083229658901644, 8.793699443340302 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-15",
					"justification" : 1,
					"maxclass" : "live.line",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 103.66803352534771, 433.585807244777698, 5.0, 100.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 33.162838566899268, 24.574308612346641, 10.638124242424965, 40.506631791591644 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-14",
					"justification" : 1,
					"maxclass" : "live.line",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 87.779824525117874, 433.585807244777698, 5.0, 100.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 32.93786896020174, 20.757288568615905, 112.112351968586381, 8.521357856690884 ]
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.2, 0.2, 0.2, 0.0 ],
					"id" : "obj-9",
					"maxclass" : "led",
					"numinlets" : 1,
					"numoutlets" : 1,
					"oncolor" : [ 0.270588235294118, 0.76078431372549, 0.172549019607843, 1.0 ],
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 234.353125423192978, 358.087605297565517, 24.0, 24.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 127.050220928788121, 77.666655109226696, 18.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.2, 0.2, 0.2, 0.0 ],
					"id" : "obj-4",
					"maxclass" : "led",
					"numinlets" : 1,
					"numoutlets" : 1,
					"oncolor" : [ 0.270588235294118, 0.76078431372549, 0.172549019607843, 1.0 ],
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 239.353125423192978, 293.099099099636078, 24.0, 24.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 127.050220928788121, 58.684090682268135, 18.0, 18.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-31",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 66.692927002906799, 316.099099099636078, 29.5, 22.0 ],
					"text" : "!= 1"
				}

			}
, 			{
				"box" : 				{
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"enablehscroll" : 0,
					"enablevscroll" : 0,
					"id" : "obj-28",
					"lockeddragscroll" : 0,
					"lockedsize" : 0,
					"maxclass" : "bpatcher",
					"name" : "M4L.IO.Remote.maxpat",
					"numinlets" : 3,
					"numoutlets" : 2,
					"offset" : [ 0.0, 0.0 ],
					"outlettype" : [ "int", "" ],
					"patching_rect" : [ 157.14870810508728, 390.134584954977072, 101.204417318105698, 18.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 143.999304286217694, 79.166655109226696, 95.0, 15.0 ],
					"viewvisibility" : 1
				}

			}
, 			{
				"box" : 				{
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"enablehscroll" : 0,
					"enablevscroll" : 0,
					"id" : "obj-18",
					"lockeddragscroll" : 0,
					"lockedsize" : 0,
					"maxclass" : "bpatcher",
					"name" : "M4L.IO.Remote.maxpat",
					"numinlets" : 3,
					"numoutlets" : 2,
					"offset" : [ 0.0, 0.0 ],
					"outlettype" : [ "int", "" ],
					"patching_rect" : [ 157.14870810508728, 327.08760529756546, 101.204417318105698, 18.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 143.999304286217694, 60.184090682268135, 95.0, 15.0 ],
					"viewvisibility" : 1
				}

			}
, 			{
				"box" : 				{
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"enablehscroll" : 0,
					"enablevscroll" : 0,
					"id" : "obj-24",
					"lockeddragscroll" : 0,
					"lockedsize" : 0,
					"maxclass" : "bpatcher",
					"name" : "M4L.IO.Remote.maxpat",
					"numinlets" : 3,
					"numoutlets" : 2,
					"offset" : [ 0.0, 0.0 ],
					"outlettype" : [ "int", "" ],
					"patching_rect" : [ 157.14870810508728, 97.822609522342674, 101.204417318105698, 18.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 143.999304286217694, 16.329185405605315, 95.0, 15.0 ],
					"viewvisibility" : 1
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-2",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 66.692927002906799, 240.0, 42.0, 22.0 ],
					"text" : "> 0. 1."
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-8",
					"maxclass" : "live.dial",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 128.638339787721634, 174.563536733388901, 41.0, 48.0 ],
					"presentation" : 1,
					"presentation_rect" : [ 44.051114709973319, 41.8637023577094, 41.0, 48.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_initial" : [ 0.5 ],
							"parameter_initial_enable" : 1,
							"parameter_longname" : "live.dial[1]",
							"parameter_mmax" : 1.0,
							"parameter_shortname" : "threshold",
							"parameter_type" : 0,
							"parameter_unitstyle" : 1
						}

					}
,
					"varname" : "live.dial[1]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-6",
					"maxclass" : "live.dial",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "float" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 66.692927002906799, 174.563536733388901, 41.0, 48.0 ],
					"presentation" : 1,
					"presentation_rect" : [ -2.060612881481632, 0.957287886142723, 41.0, 48.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_longname" : "live.dial",
							"parameter_mmax" : 1.0,
							"parameter_shortname" : "input",
							"parameter_type" : 0,
							"parameter_unitstyle" : 1
						}

					}
,
					"varname" : "live.dial"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-24", 2 ],
					"order" : 0,
					"source" : [ "obj-1", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-6", 0 ],
					"order" : 1,
					"source" : [ "obj-1", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-33", 0 ],
					"order" : 0,
					"source" : [ "obj-12", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-5", 0 ],
					"order" : 1,
					"source" : [ "obj-12", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-13", 2 ],
					"source" : [ "obj-17", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-20", 2 ],
					"source" : [ "obj-19", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-3", 0 ],
					"order" : 0,
					"source" : [ "obj-2", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 0 ],
					"order" : 2,
					"source" : [ "obj-2", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-4", 0 ],
					"order" : 1,
					"source" : [ "obj-2", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-19", 0 ],
					"source" : [ "obj-21", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 0 ],
					"source" : [ "obj-27", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-12", 0 ],
					"source" : [ "obj-3", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-27", 0 ],
					"source" : [ "obj-3", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-9", 0 ],
					"source" : [ "obj-31", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-22", 2 ],
					"source" : [ "obj-32", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-26", 2 ],
					"source" : [ "obj-33", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-18", 2 ],
					"source" : [ "obj-4", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"order" : 1,
					"source" : [ "obj-5", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-21", 0 ],
					"order" : 0,
					"source" : [ "obj-5", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-2", 0 ],
					"source" : [ "obj-6", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-2", 1 ],
					"source" : [ "obj-8", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-28", 2 ],
					"source" : [ "obj-9", 0 ]
				}

			}
 ],
		"parameters" : 		{
			"obj-13::obj-32::obj-15" : [ "Unmap[3]", "unmap", 0 ],
			"obj-13::obj-32::obj-19" : [ "Map[3]", "Map", 0 ],
			"obj-18::obj-32::obj-15" : [ "Unmap[1]", "unmap", 0 ],
			"obj-18::obj-32::obj-19" : [ "Map[1]", "Map", 0 ],
			"obj-20::obj-32::obj-15" : [ "Unmap[4]", "unmap", 0 ],
			"obj-20::obj-32::obj-19" : [ "Map[4]", "Map", 0 ],
			"obj-22::obj-32::obj-15" : [ "Unmap[5]", "unmap", 0 ],
			"obj-22::obj-32::obj-19" : [ "Map[5]", "Map", 0 ],
			"obj-24::obj-32::obj-15" : [ "Unmap[2]", "unmap", 0 ],
			"obj-24::obj-32::obj-19" : [ "Map[2]", "Map", 0 ],
			"obj-26::obj-32::obj-15" : [ "Unmap[6]", "unmap", 0 ],
			"obj-26::obj-32::obj-19" : [ "Map[6]", "Map", 0 ],
			"obj-28::obj-32::obj-15" : [ "Unmap[22]", "unmap", 0 ],
			"obj-28::obj-32::obj-19" : [ "Map[23]", "Map", 0 ],
			"obj-6" : [ "live.dial", "input", 0 ],
			"obj-8" : [ "live.dial[1]", "threshold", 0 ],
			"parameterbanks" : 			{

			}
,
			"parameter_overrides" : 			{
				"obj-13::obj-32::obj-15" : 				{
					"parameter_longname" : "Unmap[3]"
				}
,
				"obj-13::obj-32::obj-19" : 				{
					"parameter_longname" : "Map[3]"
				}
,
				"obj-18::obj-32::obj-15" : 				{
					"parameter_longname" : "Unmap[1]"
				}
,
				"obj-18::obj-32::obj-19" : 				{
					"parameter_longname" : "Map[1]"
				}
,
				"obj-20::obj-32::obj-15" : 				{
					"parameter_longname" : "Unmap[4]"
				}
,
				"obj-20::obj-32::obj-19" : 				{
					"parameter_longname" : "Map[4]"
				}
,
				"obj-22::obj-32::obj-15" : 				{
					"parameter_longname" : "Unmap[5]"
				}
,
				"obj-22::obj-32::obj-19" : 				{
					"parameter_longname" : "Map[5]"
				}
,
				"obj-24::obj-32::obj-15" : 				{
					"parameter_longname" : "Unmap[2]"
				}
,
				"obj-24::obj-32::obj-19" : 				{
					"parameter_longname" : "Map[2]"
				}
,
				"obj-26::obj-32::obj-15" : 				{
					"parameter_longname" : "Unmap[6]"
				}
,
				"obj-26::obj-32::obj-19" : 				{
					"parameter_longname" : "Map[6]"
				}

			}
,
			"inherited_shortname" : 1
		}
,
		"dependency_cache" : [ 			{
				"name" : "M4L.IO.MapButton.maxpat",
				"bootpath" : "~/Desktop/20250430-katharina-stoll/spaghettimonster",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "M4L.IO.MapFunction.maxpat",
				"bootpath" : "~/Desktop/20250430-katharina-stoll/spaghettimonster",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "M4L.IO.Remote.maxpat",
				"bootpath" : "~/Desktop/20250430-katharina-stoll/spaghettimonster",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
 ],
		"autosave" : 0,
		"styles" : [ 			{
				"name" : "AudioStatus_Menu",
				"default" : 				{
					"bgfillcolor" : 					{
						"angle" : 270,
						"autogradient" : 0,
						"color" : [ 0.294118, 0.313726, 0.337255, 1 ],
						"color1" : [ 0.454902, 0.462745, 0.482353, 0 ],
						"color2" : [ 0.290196, 0.309804, 0.301961, 1 ],
						"proportion" : 0.39,
						"type" : "color"
					}

				}
,
				"parentstyle" : "",
				"multi" : 0
			}
 ]
	}

}
